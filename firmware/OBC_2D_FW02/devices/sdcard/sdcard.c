#include "os.h"
#include "mfifo.h"
#include "config.h"
#include "binaryobjects.h"
#include "fprint.h"
#include "sdcard.h"
#include "diskio.h"
#include "led_switch.h"
#include "console.h"

//max logging directory id
#define MAX_ID	99999999

static int sdcard_errcnt = 0;
bool sdcard_enable;
static uint32_t sdcard_last_checked = 0;
bool sdcard_online;
static bool failed_prompted = false;

FATFS fs;

char logdir[16];
char fpath[_MAX_LFN];

char* mkpath(char *fname)	
{
	strcpy(fpath, logdir);
	strcat(fpath, "/");
	return strcat(fpath, fname);
}

void sdcard_reload()
{
	sdcard_enable = true;
	sdcard_online = false;
	failed_prompted = false;
	sdcard_last_checked = xTaskGetTickCount() - SDCARD_CHECK_INTERVAL * 2;
}

void sdcard_off()
{
	sdcard_enable = false;//disable re-init
	sdcard_online = false;//stop recording
	BYTE cmd = 0;
	f_mount(NULL, "", 1);
	disk_ioctl(0, CTRL_POWER, &cmd);//power off sdcard
}

int sdcard_detection()
{
	if(!sdcard_enable) return 0; 
	if(!sdcard_online)
	{
		uint32_t now = xTaskGetTickCount();
		if(now - sdcard_last_checked > SDCARD_CHECK_INTERVAL)
		{
			//try to initialize, then notify status change
			if( f_mount(&fs, "", 1) == FR_OK)
			{
				sdcard_errcnt = 0;
				printf("[mem]sdcard ok\r\n");
				cmd_prompted = false;
				failed_prompted = false;
				sdcard_online = true;
			}
			else
			{
				if(!failed_prompted)
				{
					printf("[mem]no sdcard\r\n");
					cmd_prompted = false;
					failed_prompted = true;
				}
				//led_no_sdcard();
			}
			sdcard_last_checked = now;
		}
	}
	else
	{
		//error occurred
		if(sdcard_errcnt > SDCARD_ERROR_TOLERANCE)
		{
			//pending re-initialize
			sdcard_online = false;
			sdcard_last_checked = xTaskGetTickCount() - 1000;
			//notify status change
			printf("[mem]sdcard offline\r\n");
			cmd_prompted = false;
			failed_prompted = false;
		}
	}
	if(sdcard_online == true)	return 1;
	return 0;
}

void sdcard_fsinfo()
{
	long p1, p2;
	TCHAR ptr[] = "/";
	FRESULT res;
	FATFS *fs;
	TCHAR Buff[32];
	static const BYTE ft[] = {0, 12, 16, 32};
	res = f_getfree(ptr, (DWORD*)&p1, &fs);
	if (res) { 
		printf("[mem]Error: ");
		f_printresult(res);
		return; 
	}
	printf("FAT type = FAT%u\r\nBytes/Cluster = %lu\r\nNumber of FATs = %u\r\n"
		   "Root DIR entries = %u\r\nSectors/FAT = %lu\r\nNumber of clusters = %lu\r\n"
			   "Volume start (lba) = %lu\r\nFAT start (lba) = %lu\r\nDIR start (lba,clustor) = %lu\r\nData start (lba) = %lu\r\n\r\n",
			   ft[fs->fs_type & 3], (DWORD)fs->csize * 512, fs->n_fats,
			   fs->n_rootdir, fs->fsize, (DWORD)fs->n_fatent - 2,
			   fs->volbase, fs->fatbase, fs->dirbase, fs->database);
	
	res = f_getlabel(ptr, (char*)Buff, (DWORD*)&p2);
	if (res) { printf("[mem]Error: ");f_printresult(res); return; }
	printf(Buff[0] ? "Volume name is %s\r\n" : "No volume label\r\n", (char*)Buff);
	printf("Volume S/N is %04X-%04X\r\n", (DWORD)p2 >> 16, (DWORD)p2 & 0xFFFF);
	printf("%lu KiB total disk space.\r\n%lu KiB available.\r\n",
		   (fs->n_fatent - 2) * (fs->csize / 2), (DWORD)p1 * (fs->csize / 2)
			   );
}

void sdcard_diskinfo()
{
	long p2;
	BYTE b;
	BYTE Buff[64];
	int s1;
	if (disk_ioctl(0, GET_SECTOR_COUNT, &p2) == RES_OK) {
		printf("Drive size: %lu sectors\r\n", p2);
	}
	if (disk_ioctl(0, GET_BLOCK_SIZE, &p2) == RES_OK) {
		printf("Block size: %lu sectors\r\n", p2); 
	}
	if (disk_ioctl(0, MMC_GET_TYPE, &b) == RES_OK)	{ 
		printf("Media type: %u\r\n", b); 
	}
	if (disk_ioctl(0, MMC_GET_CSD, Buff) == RES_OK) { 
		printf("CSD: "); print_hex(Buff, 0, 16); printf("\r\n");
	}
	if (disk_ioctl(0, MMC_GET_CID, Buff) == RES_OK)	{
		printf("CID: "); print_hex(Buff, 0, 16); printf("\r\n");
	}
	if (disk_ioctl(0, MMC_GET_OCR, Buff) == RES_OK){
		printf("OCR: "); print_hex(Buff, 0, 16); printf("\r\n");
	}
	if (disk_ioctl(0, MMC_GET_SDSTAT, Buff) == RES_OK) {
		printf("SD Status:\r\n");
		for (s1 = 0; s1 < 64; s1 += 16) {
			print_hex(Buff, s1, 16);
			printf("\r\n");
		}
	}
}

void sdcard_info()
{
	sdcard_diskinfo();
	printf("\r\n");
	sdcard_fsinfo();
	printf("\r\n");
}
/*
void sdcard_ls(char *path)
{
	FRESULT res;
	long p1;
	UINT s1, s2;
	DIR Dir;					
	FILINFO Finfo;
	char lfname[_MAX_LFN];
	FATFS *fs;
	Finfo.lfname = lfname;
	
	res = f_opendir(&Dir, path);
	if (res) { 
		printf("Error: ");
		f_printresult(res);
		printf("\r\n"); 
		return; 
	}
	p1 = s1 = s2 = 0;
	for(;;) {
		res = f_readdir(&Dir, &Finfo);
		if ((res != FR_OK) || !Finfo.fname[0]) break;
		if (Finfo.fattrib & AM_DIR) {
			s2++;
		} else {
			s1++; p1 += Finfo.fsize;
		}
		printf("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %-12s  %s\r\n",
			   (Finfo.fattrib & AM_DIR) ? 'D' : '-',
			   (Finfo.fattrib & AM_RDO) ? 'R' : '-',
			   (Finfo.fattrib & AM_HID) ? 'H' : '-',
			   (Finfo.fattrib & AM_SYS) ? 'S' : '-',
			   (Finfo.fattrib & AM_ARC) ? 'A' : '-',
			   (Finfo.fdate >> 9) + 1980, (Finfo.fdate >> 5) & 15, Finfo.fdate & 31,
			   (Finfo.ftime >> 11), (Finfo.ftime >> 5) & 63,
			   Finfo.fsize, Finfo.fname, Finfo.lfname);
	}
	printf("%4u File(s),%10lu bytes total\r\n%4u Dir(s)", s1, p1, s2);
	res = f_getfree(path, (DWORD*)&p1, &fs);
	if (res == FR_OK)
		printf(", %10lu bytes free\r\n", p1 * fs->csize * 512);
	else
	{
		printf("Error: ");
		f_printresult(res); 
		printf("\r\n");
	}
}
*/
void sdcard_cd(char *path)
{
	FRESULT res = f_chdir(path);
	if(res)
	{
		printf("Error: ");
		f_printresult(res);
		printf("\r\n");
	}
}

void sdcard_showcwd()
{
	char cwd[_MAX_LFN];
	f_getcwd(cwd, _MAX_LFN);
	printf("%s\r\n", cwd);
}

void sdcard_mkdir(char *path)
{
	FRESULT res = f_mkdir(path);
	if(res)
	{
		printf("Error: ");
		f_printresult(res);
		printf("\r\n");
	}
}

FRESULT verbose_open (FIL* fp, const TCHAR* path, BYTE mode)
{
	FRESULT res = f_open(fp, path, mode);
	if(res) {
		printf("Error opening %s : ", path );
		f_printresult(res);
		printf("\r\n");
		sdcard_reload();
	}
	return res;
}

FRESULT verbose_flush (FIL* fp)
{
	FRESULT res = f_sync(fp);
	if(res) {
		printf("Error flushing : ");
		f_printresult(res);
		printf("\r\n");
		sdcard_reload();
	}
	return res;
}

void sdcard_format(uint16_t alloc_unit)
{
	//mkfs
	char path[] = "/";
	printf("[mem]format disk\r\n");
	printf("ALL data will lost, continue? (y/n)=");
	while(getchar() >= 0);
	int c0 = 0, c1 = 0, c2;
	while(1)
	{
		c2 = getchar();
		if(c2 < 0) continue;
		c0 = c1;
		c1 = c2;
		if(isalpha(c0) && (c1 == '\r' || c1 == '\n'))
		{
			if(c0 == 'y' || c0 == 'Y')
			{
				//format disk
				printf("formatting...");
				FRESULT res = f_mkfs(path, 0, alloc_unit);
				if(res == FR_OK)
				{
					printf("ok, ");
					DWORD n;
					FATFS *fs;
					res = f_getfree(path, &n, &fs);
					if (res == FR_OK) 
						printf("%luMiB avaliable", n * fs->csize / 2048);
					printf("\r\n");
				}
				else
				{
					printf("Error: ");
					f_printresult(res);
					printf("\r\n");
				}
			}
			else
			{
				//format cancelled
				printf("Cancelled\r\n");
			}
			break;
		}
	}
}

void sdcard_reader(char *file)
{
	uint32_t dump_timer;
	FIL fp;
	FRESULT res = f_open(&fp, file, FA_READ);
	if(res) {
		printf("Error opening file: ");
		f_printresult(res);
		printf("\r\n");
		return;
	}
	dump_timer = xTaskGetTickCount();
	char buf[512];
	UINT read;
	while(1)
	{
		res = f_read(&fp, buf, sizeof(buf), &read);
		if(res)
		{
			//putchar(26);
			printf("ERROR reading file: ");
			f_printresult(res);
			printf("\r\n");
			break;
		}
		for(UINT i=0;i<read;i++)
			putchar(buf[i]);
		if(sizeof(buf) != read)
		{
			//putchar(26);
			printf("[mem]EOF\r\n");
			break;
		}
		if(xTaskGetTickCount() - dump_timer > 30000)
		{
			//putchar(26);
			printf("[mem]TIMEOUT\r\n");
			break;
		}
	}
	f_close(&fp);
}

void sdcard_writer(char *file)
{
	FIL fp;
	uint32_t last_write = xTaskGetTickCount();
	FRESULT res = f_open(&fp, file, FA_WRITE | FA_OPEN_ALWAYS);
	if(res) {
		printf("Error opening file: ");
		f_printresult(res);
		printf("\r\n");
		return;
	}
	f_lseek(&fp, f_size(&fp));
	char buf[1024];
	int bufptr = 0;
	UINT written;
	while(1)
	{
		int c = getchar();
		if(c == 26)
		{
			printf("EOF\r\n");
			break;
		}
		if(c < 0)
		{
			//stdin is idle, flush buffer
			res = f_write(&fp, buf, bufptr, &written);
			last_write = xTaskGetTickCount();
			bufptr = 0;
		}
		else
		{
			//save char
			buf[bufptr++] = c;
			if(c == '\r')
				buf[bufptr++] = '\n';
			if((xTaskGetTickCount() - last_write > 100) || bufptr >= sizeof(buf)-1)
			{
				//no write for a long time, flush buffer
				res = f_write(&fp, buf, bufptr, &written);
				last_write = xTaskGetTickCount();
				bufptr = 0;
			}
		}
		if(res)
		{
			printf("ERROR writing file: ");
			f_printresult(res);
			printf("\r\n");
			break;
		}
	}
	
	res = f_write(&fp, buf, bufptr, &written);
	if(res)
	{
		printf("ERROR writing file: ");
		f_printresult(res);
		printf("\r\n");
	}
	res = f_close(&fp);
	if(res)
	{
		printf("ERROR writing file: ");
		f_printresult(res);
		printf("\r\n");
	}
	else
		printf("File saved\r\n");
}

void sdcard_remove(char *file)
{
	FRESULT res = f_unlink(file);
	if(res)
	{
		printf("Error deleting file: ");
		f_printresult(res);
		printf("\r\n");
	}
	//else
	//	printf("Delete ok\r\n");
}

int sdcard_execute(char *file)
{
	
	FIL fp;
	FRESULT res = f_open(&fp, file, FA_READ);
	if(res) {
		printf("Error opening file: ");
		f_printresult(res);
		printf("\r\n");
		if(res == FR_NO_FILE)
			return -1;
		else
		{
			sdcard_reload();
			return -2;
		}
	}
	char buf[512];
	while(!f_eof(&fp))
	{
		char *p = f_gets(buf, sizeof(buf), &fp);
		if(!p)
		{
			if(f_error(&fp))
			{
				printf("ERROR reading file: ");
				f_printresult((FRESULT)f_error(&fp));
				printf("\r\n");
			}
			break;
		}
		buf[sizeof(buf)-1] = '\0';
		cmd_parse_line(buf);
	}
	f_close(&fp);
	return 0;
}


uint8_t sdcard_wcsplog(const char *filename,csp_packet_t *packet)
{
	FIL f;
	uint32_t length;
	if(f_open(&f,filename,FA_WRITE|FA_OPEN_EXISTING))
	{
		return 0;
	}
	if(f_lseek(&f,f.fsize))
	{
		return 0;
	}
	if(f_write(&f,packet->data,packet->length,(UINT*)&length))
	{
		printf("CSP Recieve Packet write to log fail\n");
		return 0;
	}
	f_close(&f);
	return 1;
}

uint8_t sdcard_wlog(FIL *f,uint8_t *data,uint32_t size)
{
	uint32_t length;
	
	if(f_write(f,data,size,(UINT*)&length))
	{
		printf("CSP Recieve Packet write to log fail\n");
		return 0;
	}
	f_sync(f);
	return 1;
}

