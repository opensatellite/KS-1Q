#ifndef __IMUD_H__
#define __IMUD_H__

#include "AHRS/3dmath_helper.h"
#include "adis16375.h"
//#include "hmc5883l.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define IMU_SAMPLE_RATE         (IMUD_SENSOR_SAMPLERATE/IMUD_SENSOR_DECIM)

//-----------------@binaryobjects#start----------------------------
typedef struct {
    uint32_t timestamp;
    vec3f gyro;//rad/s
    vec3f body_acc;//g
    float temp;//`C
    uint16_t status;//imu status bits
}imu_data_t; 

typedef struct {
    uint32_t timestamp;
    vec3f h;
}mag_data_t;
//-----------------@binaryobjects#end----------------------------

void imumagd_thread(void  *args);

void imumagd_start();
void imumagd_stop();

#ifdef __cplusplus
}
#endif

#endif
