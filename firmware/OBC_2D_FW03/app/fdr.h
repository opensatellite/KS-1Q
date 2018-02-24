#ifndef _FDR_H__
#define _FDR_H__

//-----------------@binaryobjects#start----------------------------
#define FDR_IMU_FAST   0
#define FDR_IMU_MED    1
#define FDR_MAG        2
#define FDR_BARO       3
#define FDR_NAV        4
#define FDR_GPS        5
#define FDR_GPS_ECEF   6
#define FDR_TASKMON    7
#define FDR_STATUS     29
#define FDR_MARK       30
#define FDR_INSERT     31
//-----------------@binaryobjects#end----------------------------

//logging channels
typedef struct {
    uint8_t id;
    float rate;
    void (*callback)(uint32_t,int);
    uint32_t interval_us;
    uint32_t last_check;
    uint32_t last_timestamp;
}fdr_channel_t;

//define logging function in fdr_logs.c
typedef struct {
    uint64_t options;//enabled channels (1<<ID)|(1<<ID2)
    int buf_size;//1024,2048,4096...
    int max_retry;
    bool keep_recording;//try to continue recording when sdcard back
}fdr_config_t;
#define FDR_ALL           (0xffffffff)  //enable all channels 

//-----------------@binaryobjects#start----------------------------
#define FDR_ERR_PARAM   -1       //start parameter error
#define FDR_NO_DISK     -2       //no card inserted
#define FDR_FS_ERR      -3       //filesystem error
#define FDR_NO_FNAME    -4       //too much log file
#define FDR_OPEN_ERR    -5       //cannot open log file
#define FDR_DISK_FULL   -6       //no space left on disk
#define FDR_WRITE_ERR   -7       //error writing file
#define FDR_OVERRUN     -8       //buffer overflow, overrun occurred
#define FDR_NO_MEM      -9       //system memory not enough
//-----------------@binaryobjects#end----------------------------

void fdr_start(fdr_config_t *cfg);
void fdr_stop();
bool fdr_check(uint8_t id);//return true if something is recorded
//void fdr_log(uint8_t id, ...);

bool fdr_is_running();
bool fdr_disk_full();
uint32_t fdr_last_write();//last write operation in ms
int fdr_last_err();
uint32_t fdr_total_written();
bool fdr_enabled();
bool fdr_keep_recording();

/*
    FDR Packet Format
    |Header | Type |     Size    |    Payload   | Checksum  |
    | C5 A3 | type | N(L8) N(H8) | {Data, len=N}| CK_A CK_B |
            |<-------------Checksum------------>|
*/

void fdr_mark(char *notes);//insert a string
void fdr_insert(uint8_t *data,size_t len);//insert data

#endif