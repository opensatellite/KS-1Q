#pragma once

extern FATFS fs;

char* mkpath(char *fname);

extern bool sdcard_enable;
int sdcard_detection();
void sdcard_info();
void sdcard_fsinfo();
void sdcard_diskinfo();
void sdcard_cd(char *path);
void sdcard_ls(char *path);
void sdcard_showcwd();
void sdcard_mkdir(char *path);
void sdcard_format(uint16_t alloc_unit);

void sdcard_reload();//trigger sdcard reload
void sdcard_off();//set sdcard offline

void sdcard_reader(char *file);//read and print file
void sdcard_writer(char *file);//write stdin to file, stop at Ctrl-Z
void sdcard_remove(char *file);//remove a file or a empty directory
int  sdcard_execute(char *file);//execute script from card, 0 for ok, -1 for no file, -2 for error


uint8_t sdcard_wcsplog(const char *filename,csp_packet_t *packet);
uint8_t sdcard_wlog(FIL *f,uint8_t *data,uint32_t size);