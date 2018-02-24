/*
  Copyright (C) 2008. LIS Laboratory, EPFL, Lausanne
-Edit by WarMonkey(luoshumymail@gmail.com) for project KCNAV2C

  This file is part of Aeropic.

  Aeropic is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 2.1 of the License, or
  (at your option) any later version.

  Aeropic is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  aint32_t  with Aeropic.  If not, see <http://www.gnu.org/licenses/>.
*/ 
/*!
*	\file gps_ubx.h
*	\brief Header file of the GPS UBX binary protocol decoding module
*/

#ifndef __GPS_UBX_H__
#define __GPS_UBX_H__

#include "datatypes.h"

//--------------------
// Public definitions
//--------------------

//! Number of times ubx_CheckTimeout() must be called without response from GPS before it is considered as timed out
//#define UBX_TIMEOUT_CYCLES 2
//! The minimum precision to consider a position as correct (in mm)
#define UBX_POSITION_PRECISION 20000
//! The minimum precision to consider an altitude as correct (in mm)
#define UBX_ALTITUDE_PRECISION 20000
//! The minimum precision to consider a speed as correct (in cm/s)
#define UBX_SPEED_PRECISION 500
//! The minimum precision to consider a heading as correct (in deg*10^5)
#define UBX_HEADING_PRECISION 3000000

// Ids
//! The id value for NAV-POSLLH messages
#define UBX_ID_NAV_POSLLH 0x02
//! The id value for NAV-VELNED messages
#define UBX_ID_NAV_VELNED 0x12
//! The id value for NAV-STATUS messages
#define UBX_ID_NAV_STATUS 0x03
//! The id value for NAV-SVINFO 
#define UBX_ID_NAV_SVINFO 0x30
//! The id value for NAV-SOL
#define UBX_ID_NAV_SOL    0x06
//! The id value for NAV-TIMEUTC
#define UBX_ID_NAV_TIMEUTC    0x21


//-------------------------
// Public type definitions
//-------------------------

//! The structure for UBX protocol, Posllh messages
typedef struct
{
	uint32_t itow;
	int32_t  lon;
	int32_t  lat;
	int32_t  height;
	int32_t  hmsl;
	uint32_t hAcc;
	uint32_t vAcc;
}
ubx_MessagePosllh;

//! The structure for UBX protocol, Velned messages
typedef struct
{
	uint32_t itow;
	int32_t  velN;
	int32_t  velE;
	int32_t  velD;
	uint32_t speed;
	uint32_t gSpeed;
	int32_t  heading;
	uint32_t sAcc;
	uint32_t cAcc;
}
ubx_MessageVelned;

//! The structure for UBX protocol, Status messages
typedef struct
{
	uint32_t itow;
	uint8_t gpsFix;
	uint8_t flags;
	uint8_t diffS;
	uint8_t res;
	uint32_t ttff;
	uint32_t msss;
}
ubx_MessageStatus;


//! The structure for UBX protocol, SVInfo message
typedef struct
{
	uint32_t itow;
	uint8_t numCh;
	uint8_t globalFlags;
	uint16_t reserved;
	
	struct
	{
		uint8_t chn;
		uint8_t svid;
		uint8_t flags;
		uint8_t quality;
		uint8_t cno;
		 int8_t elev;
		int16_t azim;
		int32_t prRes;
	} channelData[16];	
}
ubx_MessageSVInfo;

//! The structure for UBX protocol, Solution message
typedef struct 
{
    uint32_t itow;
    int32_t  ftow;
    int16_t  week;
    uint8_t  gpsFix;
    uint8_t  flags;
    int32_t  ecefX;
    int32_t  ecefY;
    int32_t  ecefZ;
    uint32_t pAcc;
    int32_t  ecefVX;
    int32_t  ecefVY;
    int32_t  ecefVZ;
    uint32_t sAcc;
    uint16_t pDOP;
    uint8_t  reserved1;
    uint8_t  numSV;
    uint32_t reserved2;
}ubx_MessageSol;

//! The structure for UBX protocol, UTC Time message
typedef struct 
{
    uint32_t itow;
    uint32_t tAcc;
    int32_t nano;
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t valid;
}ubx_MessageTimeUTC;
#define UBX_NAV_TIMEUTC_VALIDUTC    (1<<0)

//----------------------------
// Public function prototypes
//----------------------------

//! This function must be called frequently to detect GPS timeout
//uint8_t ubx_CheckTimeout();

//!	This function returns if there is a timeout on the GPS
//uint8_t ubx_GetTimeout();


// Implementation of the GPS interface

//! This function must be called each time when a byte is received from communication
//! Return 0 for success, > 0 for new data id, -1 for error
int ubx_Decode(uint8_t data);


// Low-level access to received UBX messages

//! This function returns a pointer to the last NAV-POSLLH message that was received
//bool ubx_PosllhUpdated();
//void ubx_PosllhClear(); //clear update status
ubx_MessagePosllh * ubx_GetPosllh();

//! This function returns a pointer to the last NAV-VELNED message that was received
//bool ubx_VelnedUpdated();
//void ubx_VelnedClear(); //clear update status
ubx_MessageVelned * ubx_GetVelned();

//! This function returns a pointer to the last NAV-STATUS message that was received
//bool ubx_StatusUpdated();
//void ubx_StatusClear(); //clear update status
ubx_MessageStatus * ubx_GetStatus();

//! This function returns a pointer to the last NAV-SVINFO message that was received
//bool ubx_SVInfoUpdated();
//void ubx_SVInfoClear(); //clear update status
ubx_MessageSVInfo * ubx_GetSVInfo();

//! This function returns a pointer to the last NAV-SOL message that was received
//bool ubx_SolUpdated();
//void ubx_SolClear(); //clear update status
ubx_MessageSol    * ubx_GetSol();

//! This function returns a pointer to the last NAV-TIMEUTC message that was received
//bool ubx_TimeUTCUpdated();
//void ubx_TimeUTCClear(); //clear update status
ubx_MessageTimeUTC* ubx_GetTimeUTC();

#endif
