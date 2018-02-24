#ifndef BINARYOBJECTS_H
#define BINARYOBJECTS_H

#include "3dmath_helper.h"

//---------------------telemetry------------------
#define TELEMETRY_MOTION_GYRO_GAIN      100.0   //0.01dps/LSB
#define TELEMETRY_MOTION_ACC_GAIN       1000.0  //0.001g /LSB
#define TELEMETRY_MOTION_MAG_GAIN       100.0   //0.01uT /LSB

//------------------channels------------------------
#define TELEMETRY_MOTION_GYRO_GAIN      100.0   //0.01dps/LSB
#define TELEMETRY_MOTION_ACC_GAIN       1000.0  //0.001g /LSB
#define TELEMETRY_MOTION_MAG_GAIN       100.0   //0.01uT /LSB
#define TELEMETRY_STATUS                0   	//timestamp,status,power
#define TELEMETRY_MOTION                1   	//timestamp,gyro,acc,magn,imu_temp
#define TELEMETRY_GPS               	2   	//timestamp,lat,lon,alt,sats
#define TELEMETRY_NAVIGATION	        3       //timestamp,att(q),vel,pos
#define TELEMETRY_BARO                  4       //timestamp,pressure,temperature
#define TELEMETRY_ATMOS                 5       //timestamp,total P/T, static P/T, alpha, beta
#define TELEMETRY_QUATERNION            6       //timestamp,quaternion attitude
#define TELEMETRY_EXTADC0               7       //timestamp,adc group 0 ch1-n
#define TELEMETRY_SERIAL0               8       //timestamp,external serial port 0
#define TELEMETRY_EXTADC1               9       //timestamp,adc group 1 ch1-n
#define TELEMETRY_ENGINECTRL            10      //timestamp,engine command
#define TELEMETRY_ENGINEPARAM		11	//timestamp,engine engineering parameters
#define TELEMETRY_ENGINETLM             12      //timestamp,engine sensor measurements
#define TELEMETRY_EPSTLM                13      //timestamp,electrical power system parameters
#define TELEMETRY_EPSCTRL               14      //timestamp,eps control

#define TELEMETRY_FDR                   28      //timestamp,flight data recorder status
#define TELEMETRY_TASKMON               29      //timestamp,task monitor
#define TELEMETRY_MARK                  30      //timestamp,time mark id
#define TELEMETRY_DEBUG                 31      //timestamp,debug message

//------------------------fdr---------------------
#define FDR_IMU_FAST   0
#define FDR_IMU_MED    1
#define FDR_MAG        2
#define FDR_BARO       3
#define FDR_NAV        4
#define FDR_GPS        5
#define FDR_GPS_ECEF   6
#define FDR_STATUS     29
#define FDR_MARK       30
#define FDR_INSERT     31

#define FDR_RUNNING    0
#define FDR_DISKFULL   1
#define FDR_ENABLE     2
#define FDR_KEEP_REC   3

//------------------------barometer---------------
typedef struct
{
    uint32_t timestamp;
    float pressure;
    float temperature;
} baro_data_t;

//------------------------imud--------------------
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

//------------------------gpsd--------------------
#define GPS_FIX_NO      0
#define GPS_FIX_DR      1       //Dead Reckoning only
#define GPS_FIX_2D      2       //2d fix
#define GPS_FIX_3D      3       //3d fix
#define GPS_FIX_GPSDR   4       //GPS+DR
#define GPS_FIX_TIME    5       //time only fix

//flags
#define GPS_FIXOK       (1<<0)
#define GPS_DIFFSOL     (1<<1)
#define GPS_WKNSET      (1<<2)  //week number valid
#define GPS_TOWSET      (1<<3)  //time of week valid

typedef struct
{
    uint32_t timestamp;
    //from NAV-SOL
    uint32_t itow;//gps millisecond time of week
    uint32_t ftow;//gps fractional nanoseconds remainder
    //from NAV-POSLLH
    int32_t lon;//longitude: 1e-7,+-180deg
    int32_t lat;//latitude:  1e-7,+-90deg
    int32_t alt;//altitude:  mm
    //from NAV-VELNED
    uint32_t gs;//ground speed: cm/s
    int32_t vs;//vertical speed: cm/s
    int16_t heading;//heading: 1e-2,+-180deg
    //from NAV-SOL
    uint8_t num_sv;//satellites used
} gps_navigation_data_t; //23bytes

//all from NAV-SOL
typedef struct
{
    uint32_t timestamp;
    uint32_t itow;//gps millisecond time of week
    int32_t  ftow;//gps fractional nanoseconds remainder
    vec3l ecef_pos;//position in ecef axes
    uint32_t pos_accuracy;
    vec3l ecef_vel;//velocity in ecef axes
    uint32_t vel_accuracy;
    uint16_t pDOP;//position DOP
    uint8_t  num_sv;
    uint8_t  gpsfix;
    uint8_t  flags;
} gps_solution_data_t;

//------------------engine ctrl------------------------------
#define ENGINECTRL_O_PRESSURIZE     0       //氧化剂储箱增压
#define ENGINECTRL_O_VENT           1       //氧化剂储箱泄放
#define ENGINECTRL_F_PRESSURIZE     2       //燃料储箱增压
#define ENGINECTRL_F_VENT           3       //燃料储箱泄放
#define ENGINECTRL_START	        4       //发动机启动
#define ENGINECTRL_STOP             5       //发动机关机
#define ENGINECTRL_MANUAL_PURGE     6       //手动吹除
#define ENGINECTRL_CLOSED_LOOP      7       //闭环推力/混合比控制

typedef struct {
    uint32_t timestamp;

    //bitfield
    uint16_t ops;

    //closed loop mode, thurst and mix ratio setpoints
    uint16_t thurst;//thurst level, 0 for 0%, 10000 for 100%
    uint16_t mixratio;//O/F mass flow, 0 for 0.000, 10000 for 10.000

    //open loop mode, valve setpoints
    uint16_t o_valve;//Oxidizer valve opening, 0 for 0%, 10000 for 100%
    uint16_t f_valve;//Fuel valve opening, 0 for 0%, 10000 for 100%

    //gimbal position
    int16_t x_pos;//0 for 0.00deg, -30000 for -30.000deg, +30000 for +30.000deg
    int16_t y_pos;

    //exhaust nozzle direction (roll control)
    int16_t z_pos;//0 for 0.00deg, -30000 for -30.000deg, +30000 for +30.000deg
} engine_ctrl_data_t;

//------------------status bits------------------------------
#define STATUS_IMU_OVERFLOW     0      //imu sensor overflow
#define STATUS_IMU_TEMPLOW      1
#define STATUS_IMU_TEMPHIGH     2
#define STATUS_IMU_STSERR       3      //imu selftest error
#define STATUS_MAG_OVERFLOW     4

#define STATUS_GPS_LOCK         8
#define STATUS_GPS_DIFF         9      //differential or enhanced gps

#define STATUS_FDR_RUNNING      16
#define STATUS_FDR_ERASING      17      //use with spiflash
#define STATUS_FDR_DISKFULL     18
#define STATUS_FDR_DISKERR      19
#define STATUS_FDR_OVERRUN      20

#define STATUS_BARO_PRESLOW     21
#define STATUS_BARO_PRESHIGH    22
#define STATUS_BARO_TEMPLOW     23
#define STATUS_BARO_TEMPHIGH    24

#define STATUS_VCC_LOW          25
#define STATUS_VCC_HIGH         26

#define STATUS_CANBUS_ONLINE    27

#define STATUS_PARACHUTE        30
#define STATUS_RUN              31

#define STATUS_IMU_TEMP_GAIN    100.0

#define FAIL_SYSTEM             32
#define FAIL_ACC                33
#define FAIL_GYRO               34
#define FAIL_MAG                35
#define FAIL_PRESSURE           36
#define FAIL_GNSS               37
#define FAIL_MEMORY             38

#define FILTER_GRAVITY          48
#define FILTER_COMPASS          49
#define FILTER_BARO_ALT         50
#define FILTER_GNSS             51
#define FILTER_ZUPT             52
#define FILTER_EXT              53

#define SYSTEM_STATUS_BITS      32
#define SYSTEM_STATUS_BYTES     4

#endif // BINARYOBJECTS_H
