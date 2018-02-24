#include <os.h>

#include "spi_flash.h"
#include "sdcard.h"

#include "ff.h"

#include "bsp.h"

#define MAX_FILEID			 (uint32_t)65535

//-----------------@binaryobjects#start----------------------------
#define MEM_ERR_PARAM   -1       //start parameter error
#define MEM_NO_DISK     -2       //no card inserted
#define MEM_FS_ERR      -3       //filesystem error
#define MEM_NO_FNAME    -4       //too much log file
#define MEM_OPEN_ERR    -5       //cannot open log file
#define MEM_DISK_FULL   -6       //no space left on disk
#define MEM_WRITE_ERR   -7       //error writing file
#define MEM_OVERRUN     -8       //buffer overflow, overrun occurred
#define MEM_NO_MEM      -9       //system memory not enough

static volatile int last_error = 0;
char filename[8];

uint32_t mem_init()
{	
	dm_var.memstatus = 0;
	dm_var.memavail = 0;
	if(sdcard_init())
	{
		dm_var.memstatus = 1;
		dm_var.memavail = 1;
	}
	//else if(spi_flash_init())
	//{
	//	dm_var.memavail = 1;
	//	dm_var.memstatus = 2;
	//}
	if(dm_var.memstatus == 1)
	{
		printf("Memory stroge now is SD Card\n");
		return 1;
	}
	else if(dm_var.memstatus == 2)
	{
		printf("Memory stroge now is SPI Flash\n");
		return 2;
	}
	dm_var.memstatus = 0;
	return 0;
}

bool openfile(FIL *f)
{
	if((dm_var.memstatus == 1) && (dm_var.memavail == 1))
	{
		//8.3 format / NO LONG FILE NAME SUPPORT
		FRESULT res;
		uint32_t f_id;
		char fname[9];
		
		f_id = 0;
		
		while(f_id <= MAX_FILEID)// from 1 to FDR_MAX_FILEID
		{
			sprintf(fname,"%d",f_id);//make filename
			printf("try filename %s \n",fname);
			res = f_open(f,fname,FA_READ|FA_OPEN_EXISTING);
			//try to open that file
			switch(res)
			{
				case FR_NO_FILE:
				//this filename is available
				f_close(f);
				//open output file 
				res = f_open(f,fname,FA_WRITE|FA_CREATE_NEW);
				if(res != FR_OK)//catched an error
				{
					last_error = MEM_OPEN_ERR;
					printf("Error opening: %s \n",fname);
					//cannot open output file
					return false;
				}
				//f_close(f);
				printf("Write to: %s \n",fname);
				return true;//open ok
				case FR_OK:
				f_id++;
				continue;
				default: //other error occurred
				last_error = MEM_FS_ERR;
				printf("Test failed at %s \n",fname);
				//sw_error("fdr","testfile",res);
				return false;
			}
		}
		last_error = MEM_NO_FNAME;
		printf("No filename available\n");            
		return false;
	}
	return false;
}

void mem_thread(void const *args)
{ 
	mem_init();
	osThreadTerminate(0);
	/*
	for(;;)
	{
		osDelay(100);
		if(dm_var.memstatus == 1)
		{
			if(!openfile(filename))
			{
				dm_var.memstatus = 2;
				continue;
			}
			dm_var.memavail = 1;
			osThreadTerminate(0);
			continue;
		}
		else if(dm_var.memstatus == 2)
		{
			dm_var.memavail = 0;
		}
		dm_var.memavail = 0;
		osThreadTerminate(0);
	}
	*/
}

