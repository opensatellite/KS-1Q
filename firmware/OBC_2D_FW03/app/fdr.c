#include "os.h"
#include "fdr.h"
#include "stdarg.h"
#include "diskio.h"
#include "sdcard.h"

#define FDR_BUF_TIMEOUT         10
#define FDR_WRITE_RETRY_DELAY   1
#define FDR_CARD_RETRY_DELAY    200
#define FDR_FAIL_RETRY_DELAY    500     //restart after error

static osThreadId fdr_write_task;
static uint64_t log_options;//options
static osSemaphoreId fdr_write_enable; 

static uint8_t *buffer;
static osPoolId fdr_buf;
static osMutexId fdr_buffer_lock;
static volatile int rpos,wpos;
static int mask;
#define BYTES_AVAILABLE() ((wpos - rpos) & mask)

static bool fdr_running = false;
static bool logging_enable = false;
static bool keep_rec = false;
static volatile uint8_t checksum_a,checksum_b;
static volatile uint32_t last_write = 0;
static volatile uint32_t total_written;
static int max_retry = 1;
static volatile int last_error = 0;

//file system objects
//FATFS fs;
FIL f; 

void fdr_log_content(void *ptr,size_t len)
{
    for(int i=0;i<len;i++)
    {
        buffer[wpos] = ((uint8_t*)ptr)[i];
        checksum_a += ((uint8_t*)ptr)[i];
        checksum_b += checksum_a;
        int tmphead = (wpos + 1) & mask;
        if(tmphead == rpos && fdr_running)
            last_error = FDR_OVERRUN;
        wpos = tmphead;
    }
}

//return false if we cannot acquire the buffer
void fdr_log_head(uint8_t type,int size)
{
    uint8_t head[] = {0xc5,0xa3};
    fdr_log_content(head,2);
    checksum_a = 0;
    checksum_b = 0;
    fdr_log_content(&type,1);
    fdr_log_content((uint8_t*)&size,2);
}

void fdr_log_tail()
{
    uint8_t ck_a=checksum_a;
    uint8_t ck_b=checksum_b;
    fdr_log_content((uint8_t*)&ck_a,1);
    fdr_log_content((uint8_t*)&ck_b,1);
}

/*
void fdr_log(uint8_t id, ...)
{
    va_list arg;
    va_start(arg,id);
    
    va_end(arg);
}
*/
//------------------------------------------------------------------------------

fdr_channel_t fdr_channels[];
#define CALLBACK(NAME)  void NAME(uint32_t now,int index)
#define CHECK_TIMESTAMP(TIMESTAMP,CODE)    {if(fdr_channels[index].last_timestamp!=(TIMESTAMP)){CODE;\
                                            {fdr_channels[index].last_timestamp=(TIMESTAMP);}}}
#define SELF_ID         (fdr_channels[index].id)
//data was updated during last interval
#define FDR_CHANNELS_N  (sizeof(fdr_channels)/sizeof(fdr_channels[0]))

//----------------------------------data writers

#include "system.h"
CALLBACK(fdr_status)
{
    uint32_t timestamp = micros() / 1000;
    uint8_t datasize = 4+2+SYSTEM_STATUS_BYTES;//4+2+4
    
    uint16_t supply_mv = system_vcc();
    uint8_t status[SYSTEM_STATUS_BYTES];
    system_status_read(status);
    
    fdr_log_head(SELF_ID,datasize);
    fdr_log_content((void*)&timestamp,4);
    fdr_log_content((void*)&supply_mv,2);
    fdr_log_content(status,SYSTEM_STATUS_BYTES);
    fdr_log_tail();
}

#include "imumagd.h"
CALLBACK(fdr_imu)
{
    CHECK_TIMESTAMP(imu_readout.timestamp, {
        imu_data_t imu_data = imu_read(0);
		pinmode(PA11,OUTPUT);
		pinhigh(PA11);
        fdr_log_head(SELF_ID,sizeof(imu_data_t));
        fdr_log_content((uint8_t*)&imu_data,sizeof(imu_data_t));
        fdr_log_tail();
		pinlow(PA11);
    });
}

CALLBACK(fdr_mag)
{
    CHECK_TIMESTAMP(mag_readout.timestamp, {
        mag_data_t mag_data = mag_read(0);
        fdr_log_head(SELF_ID,sizeof(mag_data_t));
        fdr_log_content((uint8_t*)&mag_data,sizeof(mag_data_t));
        fdr_log_tail();
    });
}

#include "barometer.h"
CALLBACK(fdr_baro)
{
    CHECK_TIMESTAMP(baro_readout.timestamp, {
        baro_data_t baro_data = baro_read(0);
        fdr_log_head(SELF_ID,sizeof(baro_data_t));
        fdr_log_content((uint8_t*)&baro_data,sizeof(baro_data_t));
        fdr_log_tail();
    });
}

#include "navd.h"
CALLBACK(fdr_nav)
{
    
}

#include "gpsd.h"
CALLBACK(fdr_gps)
{

}

CALLBACK(fdr_gps_ecef)
{
    CHECK_TIMESTAMP(gps_solution_readout.timestamp, {
        gps_solution_data_t gps_sol_data = gps_solution_read(0);
        fdr_log_head(SELF_ID,sizeof(gps_solution_data_t));
        fdr_log_content((uint8_t*)&gps_sol_data,sizeof(gps_solution_data_t));
        fdr_log_tail();
    });
}

CALLBACK(fdr_taskmon)
{
    uint32_t total_runtime;
    uint8_t ntask = uxTaskGetNumberOfTasks();
    uint16_t nbytes = ntask * sizeof( TaskStatus_t );
    TaskStatus_t *status = pvPortMalloc( nbytes );
    if( status == NULL ) return;
    ntask = uxTaskGetSystemState( status, ntask, &total_runtime );
    
    uint8_t taskname_bytes = configMAX_TASK_NAME_LEN;
    //taskhandle + taskname + tasknumber(1) + current_state(1) + current_priority(1) + base_priority(1) + runtime_counter(4) + stack_highwatermark(2)
    uint32_t task_status_bytes = 4 + 1 + 1 + 1 + 1 + 4 + 2 + taskname_bytes;
    //timestamp + total_runtime + freemem + taskname_bytes + ntask + ntask * ( task_status )
    nbytes += 4 + 4 + 4 + 1 + 1 + ntask * task_status_bytes;
    
    fdr_log_head(SELF_ID,nbytes);
    uint32_t timestamp = micros();
    fdr_log_content(&timestamp,4);
    fdr_log_content(&total_runtime,4);
    uint32_t freemem = xPortGetFreeHeapSize();
    fdr_log_content(&freemem,4);
    fdr_log_content(&taskname_bytes,1);
    fdr_log_content(&ntask,1);
    
    for(uint8_t i = 0; i < ntask; i++)
    {
        uint32_t handleaddr = (uint32_t)status[i].xHandle;
        fdr_log_content(&handleaddr,4);
        uint8_t tasknumber = status[i].xTaskNumber;
        fdr_log_content(&tasknumber,1);
        char currentstate;
        switch( status[i].eCurrentState )
        {
        case eRunning:      currentstate = 'O';break;
        case eReady:        currentstate = 'R';break;
        case eBlocked:      currentstate = 'B';break;
        case eSuspended:    currentstate = 'S';break;
        case eDeleted:      currentstate = 'X';break;
        default:            currentstate = '?';
        }
        fdr_log_content(&currentstate,1);
        uint8_t currentprio = status[i].uxCurrentPriority;
        fdr_log_content(&currentprio,1);
        uint8_t baseprio = status[i].uxBasePriority;
        fdr_log_content(&baseprio,1);
        fdr_log_content(&status[i].ulRunTimeCounter,4);
        fdr_log_content(&status[i].usStackHighWaterMark,2);
        fdr_log_content((void*)status[i].pcTaskName,taskname_bytes);
    }
   
    fdr_log_tail();
    vPortFree( status );
}

//define internal channels here
fdr_channel_t fdr_channels[] = {        //set max rate here
    {.id=FDR_IMU_FAST, .rate=1000.0,  .callback=fdr_imu          },      //normal 400Hz
    {.id=FDR_IMU_MED,  .rate=200.0,  .callback=fdr_imu          },      //normal 100Hz
    {.id=FDR_MAG,      .rate=70.0,   .callback=fdr_mag          },      //normal 50-70Hz
    {.id=FDR_BARO,     .rate=100.0,  .callback=fdr_baro         },      //normal <100Hz
    {.id=FDR_NAV,      .rate=50.0,   .callback=fdr_nav          },      //normal 50-100Hz
    {.id=FDR_GPS,      .rate=10.0,   .callback=fdr_gps          },      //normal 2.5/10Hz
    {.id=FDR_GPS_ECEF, .rate=10.0,   .callback=fdr_gps_ecef     },      //normal 2.5/10Hz
    {.id=FDR_TASKMON,  .rate=0.1,    .callback=fdr_taskmon      },      
    {.id=FDR_STATUS,   .rate=5.0,    .callback=fdr_status       },      
};

bool fdr_check(uint8_t id)
{
    bool ret = false;
    if(!logging_enable) return false;
    if(!((1UL<<id) & log_options)) return false;
        
    //check every channel here
    uint32_t now = micros();
    for(uint8_t i=0;i<FDR_CHANNELS_N;i++)//option have 32 bits
    {
        uint8_t ch_id = fdr_channels[i].id;
        if(ch_id == id)//if this is enabled
        {
            if(osMutexWait(fdr_buffer_lock,
               (FDR_BUF_TIMEOUT/os_tick_period)) == osOK)
            {
                
                uint32_t last = fdr_channels[i].last_check;
                if(now - last >= fdr_channels[i].interval_us)
                {
                    fdr_channels[i].callback(now,i);
                    fdr_channels[i].last_check = now;
                    ret = true;
                }
                osMutexRelease(fdr_buffer_lock);
            }
        }
    }
	/*
	if(ret) 
	{
		if(BYTES_AVAILABLE() >= FDR_BLOCK_SIZE ) 
			os_sem_post(fdr_write_enable);
	}
	*/
    return ret;
}

//append data, include head and tail
void fdr_mark(char *notes)
{
    if(!logging_enable) return;
    if(log_options & (1<<FDR_MARK))
    {
        uint32_t t = micros();
        int s = strlen(notes);
        if(osMutexWait(fdr_buffer_lock,
          (FDR_BUF_TIMEOUT/os_tick_period)) == osOK)
        {
            fdr_log_head(FDR_MARK,4+s);
            fdr_log_content((uint8_t*)&t,4);
            fdr_log_content((uint8_t*)notes,s);
            fdr_log_tail();
            osMutexRelease(fdr_buffer_lock);
			if(BYTES_AVAILABLE() >= FDR_BLOCK_SIZE ) 
				osSemaphoreRelease(fdr_write_enable);
        }
    }
}

void fdr_insert(uint8_t *data,size_t len)
{
    if(!logging_enable) return;
    if(log_options & (1UL<<FDR_INSERT))
    {
        uint32_t t = micros();
        if(osMutexWait(fdr_buffer_lock,
          (FDR_BUF_TIMEOUT/os_tick_period)) == osOK)
        {
            fdr_log_head(FDR_INSERT,4+len);
            fdr_log_content((uint8_t*)&t,4);
            fdr_log_content(data,len);
            fdr_log_tail();
            osMutexRelease(fdr_buffer_lock);
			if(BYTES_AVAILABLE() >= FDR_BLOCK_SIZE ) 
				osSemaphoreRelease(fdr_write_enable);
        }
    }
}

static bool openfile(char fname[])
{
    //8.3 format / NO LONG FILE NAME SUPPORT
    FRESULT res;
    uint16_t f_id;
    char fmt[] = FDR_FILENAME;
    
    f_id = 1;
    while(f_id <= FDR_MAX_FILEID)// from 1 to FDR_MAX_FILEID
    {
        sprintf(fname,fmt,f_id);//make filename
        debug("try filename %s \n",fname);
        res = f_open(&f,fname,FA_READ|FA_OPEN_EXISTING);
        //try to open that file
        switch(res)
        {
            case FR_NO_FILE:
                //this filename is available
                f_close(&f);
                                
                //open output file 
                res = f_open(&f,fname,FA_WRITE|FA_CREATE_NEW);
                if(res != FR_OK)//catched an error
                {
                    last_error = FDR_OPEN_ERR;
		    kprintf("Error opening: %s \n",fname);
                    //cannot open output file
                    return false;
                }
                
                kprintf("Write to: %s \n",fname);
                return true;//open ok
            case FR_OK:
                f_id++;
                continue;
            default: //other error occurred
                last_error = FDR_FS_ERR;
                kprintf("Test failed at %s \n",fname);
                //sw_error("fdr","testfile",res);
                return false;
        }
    }
    last_error = FDR_NO_FNAME;
    kprintf("No filename available\n");            
    return false;
}

//------------------------------------------------------------------------------

static void freespace(void)
{
    FATFS *fs;
    DWORD fre_clust, fre_sect, tot_sect;

    /* Get volume information and free clusters of drive 1 */
    FRESULT res = f_getfree("", &fre_clust, &fs);
    if (res) return;

    /* Get total sectors and free sectors */
    tot_sect = (fs->n_fatent - 2) * fs->csize;
    fre_sect = fre_clust * fs->csize;

    /* Print the free space (assuming 512 bytes/sector) */
    printf("%lu / %lu MB\n",
            fre_sect / 2048, tot_sect / 2048);
}

static void fdr_write_thread(void const *arg)
{
    FRESULT res;
    uint8_t retry_count = max_retry;
    char fname[13];

fdr_start_here:
    printf("\n-----FLIGHT DATA RECORDER-----\n");
    
    //init tf card
    printf("Init TF Card...");
    
    uint8_t count = max_retry;
    DSTATUS s = 0xff;
    while(count--)
    {
        s = disk_initialize(0);
        if(s == 0) break;
        osDelay(FDR_CARD_RETRY_DELAY);
    }
    if(s == 0)
        printf("OK\n");
    else
    {
        printf("Error: %d\n",s);
        last_error = FDR_NO_DISK;
        goto fdr_exit;
    }
    
    //mount filesystem
    printf("Mounting fs...");
    res = f_mount(&fs,"",0);
    if(res != FR_OK)
    {
        printf("Error:%d\n",res);
        last_error = FDR_FS_ERR;
        goto fdr_exit;
    }
    else printf("OK\n");
    
    //query freespace
    freespace();//no kprintf so we don't need this
    
    //open file
    if(!openfile(fname))
    {
        //error number was assigned in function openfile()
        goto fdr_exit;
    }
    
    last_error = 0;//start logging, clear past error info
    total_written = 0;
    fdr_running = true;
	
    while(logging_enable)
    {
		//if(!os_sem_wait(fdr_write_enable,1000))
		//	continue;
		osDelay(10);
		
        //flush the buffer
        while( BYTES_AVAILABLE() >= FDR_BLOCK_SIZE )
        {
            unsigned int bytes_written;
            res = f_write(&f,(void*)(buffer+rpos),FDR_BLOCK_SIZE,&bytes_written); 
            last_write = millis();
            if(res == FR_OK && bytes_written == FDR_BLOCK_SIZE)//write ok
            {
                retry_count = max_retry;
                rpos = (rpos + FDR_BLOCK_SIZE) & mask;//set read ptr
                total_written += bytes_written;
                
#if (FDR_REPORT_BYTES > 0)
                if(total_written % FDR_REPORT_BYTES == 0)
                {
                    printf("FDR: W %ld KB\n",total_written/1024);
                }
#endif
                f_sync(&f); //try sync
            }
            else//write failed
            {
                retry_count--;
                osDelay(FDR_WRITE_RETRY_DELAY);
                if(retry_count == 0)//count reached
                {
                    if( res != FR_OK ) 
                    {
                        last_error = FDR_WRITE_ERR;
                        printf("Too much error: %d,",res);
                    }
                    else 
                    {
                        last_error = FDR_DISK_FULL;
                        printf("Disk full,");
                    }
                    goto fdr_exit;
                }
            }
        }
    }
    //clear flag
    fdr_running = false;
    //normal exit, flush buffer
    while( BYTES_AVAILABLE() > 0)
    {
        uint16_t bytes_to_write = min(FDR_BLOCK_SIZE,BYTES_AVAILABLE());
        unsigned int bytes_written;
        res = f_write(&f,(void*)(buffer+rpos),bytes_to_write,&bytes_written); 
        last_write = millis();
        if(res == FR_OK || bytes_written == FDR_BLOCK_SIZE)//write ok
        {
            rpos = ( rpos + bytes_written ) & mask;
            //set write ptr
            total_written += bytes_written;
        }
        else//write failed
        {
            retry_count--;
            osDelay(FDR_WRITE_RETRY_DELAY);
            if(retry_count == 0)//count reached
            {
                if( res != FR_OK ) 
                {
                    last_error = FDR_WRITE_ERR;
                    printf("Too much error: %d\n",res);
                }
                else 
                {
                    last_error = FDR_DISK_FULL;
                    printf("Disk full\n");
                }
                break;
            }
        }
    }
    printf("FDR: File %s total %ld bytes\n",fname,total_written);
    
    //abnormal exit
fdr_exit:    
    printf("Exiting...\n");
    //clear flag
    fdr_running = false;
    //if abnormal exit, try to restart
    if(logging_enable && keep_rec)
    {
        osDelay(FDR_FAIL_RETRY_DELAY);
        goto fdr_start_here;
    }
    //close file
    f_close(&f);
    //umount sdcard
    f_mount(NULL,"",0);
    //free buffer area
	osPoolFree (fdr_buf, buffer);
    //os_free(buffer);
    //free sem and mutex
    osMutexDelete(fdr_buffer_lock);
    osSemaphoreDelete(fdr_write_enable);
    //task exit
    osThreadTerminate(osThreadGetId());
}

void fdr_start(fdr_config_t *cfg)
{
    printf("Starting task fdr... ");
    
    rpos = wpos = 0;
    keep_rec = cfg->keep_recording;
    
    //log parameters, check every channel
    for(uint8_t i=0;i<FDR_CHANNELS_N;i++)//option have 32 bits
    {
        fdr_channels[i].interval_us = 
            (uint32_t)(1000000.0 / fdr_channels[i].rate);
        fdr_channels[i].last_check = 0;
    }
    
    mask = cfg->buf_size-1;
    //check mask
    if((mask & cfg->buf_size) != 0)
    {
        sw_error("fdr","buf_mask",mask);
        last_error = FDR_ERR_PARAM;
        return;
    }
    if(cfg->buf_size < FDR_BLOCK_SIZE)
    {
        sw_error("fdr","buf_size",cfg->buf_size);
        last_error = FDR_ERR_PARAM;
        return;
    }
    
	osPoolDef (fdr, cfg->buf_size, sizeof(uint8_t));
  	fdr_buf = osPoolCreate(osPool(fdr));
	buffer = (uint8_t *)osPoolAlloc (fdr_buf);
	
    //buffer = os_malloc(cfg->buf_size);
    if(buffer == 0)
    {
        sw_error("fdr","create_buffer",-1);
        last_error = FDR_NO_MEM;
        return;
    }
                 
    max_retry = cfg->max_retry;
    log_options = cfg->options;
    
	osMutexDef(fdr_buflock);
    fdr_buffer_lock = osMutexCreate(osMutex(fdr_buflock));
    //vQueueAddToRegistry(fdr_buffer_lock,"fdr_buflock");
    if(fdr_buffer_lock == NULL) 
    {
        //sw_error("fdr","create_mutex",-1);
        osPoolFree(fdr_buf,buffer);
        last_error = FDR_NO_MEM;
    }
    osMutexRelease(fdr_buffer_lock);
    
  	osSemaphoreDef(fdr_write_enable); 
	fdr_write_enable = osSemaphoreCreate(osSemaphore(fdr_write_enable), 1);
	vQueueAddToRegistry(fdr_write_enable,"fdr_wr_en");
	
    if(fdr_write_enable == NULL)
    {
        //sw_error("fdr","create_sem",-1);
        osMutexDelete(fdr_buffer_lock);
        osPoolFree(fdr_buf,buffer);
        last_error = FDR_NO_MEM;
    }
    
    logging_enable = true;
	
	osThreadDef(fdr_write,fdr_write_thread,FDR_WRITE_PRIO,1,160);
    fdr_write_task = osThreadCreate(osThread(fdr_write),NULL);
    if(fdr_write_task != NULL)
        printf("OK\n");
    else
    {
        osPoolFree(fdr_buf,buffer);
        osMutexDelete(fdr_buffer_lock);
        osSemaphoreDelete(fdr_write_enable);
        //sw_error("fdr","create_task",-1);
        last_error = FDR_NO_MEM;
    }
}

void fdr_stop()
{
    printf("Stopping task fdr... ");
    logging_enable = false;
}

bool fdr_is_running()
{
    return fdr_running;
}

bool fdr_disk_full()
{
    return (last_error == FDR_DISK_FULL) ? true : false;
}

uint32_t fdr_last_write()
{
    return last_write;
}

int fdr_last_err()
{
    return last_error;
}

uint32_t fdr_total_written()
{
    return total_written;
}

bool fdr_enabled()
{
    return logging_enable;
}

bool fdr_keep_recording()
{
    return keep_rec;
}
