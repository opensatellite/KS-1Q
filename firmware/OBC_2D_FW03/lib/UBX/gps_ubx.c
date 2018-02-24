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
along with Aeropic.  If not, see <http://www.gnu.org/licenses/>.
*/ 
/*!
*	\file gps_ubx.c
*	\brief Source file of the GPS UBX binary protocol decoding module
*
*
*	This file contains all the internal variables and functions of the module.
*	This module is designed to read messages of the UBX binary protocol. The messages
*	that are supported so far are : nav-posllh, nav-velned and nav-status.
*
*/

//----------
// Includes
//----------
#include "gps_ubx.h"

//---------------------
// Private definitions
//---------------------


//! The first header byte of every message sent in UBX protocol
#define UBX_HEADER1 0xb5
//! The second header byte of every message sent in UBX protocol
#define UBX_HEADER2 0x62

// Classes
//! The class value for NAV-class messages
#define UBX_CLASS_NAV 0x01

// Sizes
//! The size of the NAV-POSLLH messages
#define UBX_SIZE_NAV_POSLLH 28
//! The size of the NAV-VELNED messages
#define UBX_SIZE_NAV_VELNED 36
//! The size of the NAV-STATUS messages
#define UBX_SIZE_NAV_STATUS 16
//! The size of the NAV-SOL messages
#define UBX_SIZE_NAV_SOL    52
//! The size of the NAV-TIMEUTC messages
#define UBX_SIZE_NAV_TIMEUTC 20


//! Max number of channel to be used
#define UBX_MAX_CHANNEL 16

//---------------------------
// Private types definitions
//---------------------------

//! Enumeration of states for the receiving state machine
typedef enum
{
    UBX_STATE_HEADER1 = 0,
    UBX_STATE_HEADER2,
    UBX_STATE_CLASS,
    UBX_STATE_ID,
    UBX_STATE_LENGTH,
    UBX_STATE_CONTENT,
    UBX_STATE_CHECKSUM_A,
    UBX_STATE_CHECKSUM_B
}
ubx_State;


//-------------------
// Private variables
//-------------------

//! The current state of the finite-state-machine for the decoding of incoming messages
ubx_State ubx_state = UBX_STATE_HEADER1;
//! The class of the current message
uint8_t ubx_currentMessageClass = 0;
//! The id of the current message
uint8_t ubx_currentMessageId = 0;
//! The size of the current message
uint16_t ubx_currentMessageSize = 0;
//! The current byte of the size being received
uint8_t ubx_currentMessageSizeByte = 0;
//! The checksum A calculated of the current message
uint8_t ubx_currentMessageCkA = 0;
//! The checksum B calculated of the current message
uint8_t ubx_currentMessageCkB = 0;
//! The checksum A read for the current message
uint8_t ubx_currentMessageCkARead = 0;
//! The checksum B read for the current message
uint8_t ubx_currentMessageCkBRead = 0;

//! If a timeout occured on the GPS (GPS not connected or not working)
//uint8_t ubx_timeout = false;

//! The current size of the current message (message full if currentMessageCurrentByte == currentMessageSize)
uint16_t ubx_currentMessageCurrentByte = 0;
//! The pointer to the pointer to the structure of the current message to fill
uint8_t ** ubx_currentMessage = 0;
//! The pointer to the pointer to the structure of the last message received of the same type than the current one being received (for exchange at the end)
uint8_t ** ubx_lastMessage = 0;
//! The pointer to the number to increment when a message of the type has been received
uint16_t * ubx_validMessage = 0;
//! The pointer to the boolean to show if the message is updated
//bool * ubx_isMessageUpdated = 0;
//! The message is unknown, drop it
bool ubx_isUnknown = false;

// We are using two buffers for each message, one for the last message received, the other for the message being received (not complete)
//! The Posllh message buffer
ubx_MessagePosllh ubx_posllhMessage[2];
//! The Velned message buffer
ubx_MessageVelned ubx_velnedMessage[2];
//! The Status message buffer
ubx_MessageStatus ubx_statusMessage[2];
//! The SVInfo message buffer
ubx_MessageSVInfo ubx_svInfoMessage[2];
//! The Sol message buffer
ubx_MessageSol    ubx_solMessage[2];
//! The TimeUTC message buffer
ubx_MessageTimeUTC ubx_timeUTCMessage[2];

// NAV-POSLLH
//! The pointer to the Posllh message that is being filled (not usable)
ubx_MessagePosllh * ubx_currentPosllhMessage = &ubx_posllhMessage[0];
//! The pointer to the last Posllh message that was completed
ubx_MessagePosllh * ubx_lastPosllhMessage = &ubx_posllhMessage[1];
//! Number of valid Posllh message received
uint16_t ubx_numberOfValidPosllhMessage = 0;
//! Updated
//bool ubx_IsPosllhUpdated = false;
// NAV-VELNED
//! The pointer to the Velned message that is being filled (not usable)
ubx_MessageVelned * ubx_currentVelnedMessage = &ubx_velnedMessage[0];
//! The pointer to the last Velned message that was completed
ubx_MessageVelned * ubx_lastVelnedMessage = &ubx_velnedMessage[1];
//! Number of valid Velned message received
uint16_t ubx_numberOfValidVelnedMessage = 0;
//! Updated
//bool ubx_IsVelnedUpdated = false;
// NAV-STATUS
//! The pointer to the Status message that is being filled (not usable)
ubx_MessageStatus * ubx_currentStatusMessage = &ubx_statusMessage[0];
//! The pointer to the last Status message that was completed
ubx_MessageStatus * ubx_lastStatusMessage = &ubx_statusMessage[1];
//! Number of valid Status message received
uint16_t ubx_numberOfValidStatusMessage = 0;
//! Updated
//bool ubx_IsStatusUpdated = false;
// NAV-SVINFO
//! The pointer to the Status message that is being filled (not usable)
ubx_MessageSVInfo * ubx_currentSVInfoMessage = &ubx_svInfoMessage[0];
//! The pointer to the last Status message that was completed
ubx_MessageSVInfo * ubx_lastSVInfoMessage = &ubx_svInfoMessage[1];
//! Number of valid Status message received
uint16_t ubx_numberOfValidSVInfoMessage = 0;
//! Updated
//bool ubx_IsSVInfoUpdated = false;
// NAV-SOL
//! The pointer to the Status message that is being filled (not usable)
ubx_MessageSol * ubx_currentSolMessage = &ubx_solMessage[0];
//! The pointer to the last Status message that was completed
ubx_MessageSol * ubx_lastSolMessage = &ubx_solMessage[1];
//! Number of valid Status message received
uint16_t ubx_numberOfValidSolMessage = 0;
//! Updated
//bool ubx_IsSolUpdated = false;
// NAV-TIMEUTC
//! The pointer to the Status message that is being filled (not usable)
ubx_MessageTimeUTC * ubx_currentTimeUTCMessage = &ubx_timeUTCMessage[0];
//! The pointer to the last Status message that was completed
ubx_MessageTimeUTC * ubx_lastTimeUTCMessage = &ubx_timeUTCMessage[1];
//! Number of valid Status message received
uint16_t ubx_numberOfValidTimeUTCMessage = 0;
//! Updated
//bool ubx_IsTimeUTCUpdated = false;

//! Satellite strength buffer
uint8_t ubx_satelliteStrength[UBX_MAX_CHANNEL];
//! Number of satellite used for navigation
uint8_t ubx_satelliteCount;

// Statistics
//! The number of bytes dropped in header1 state
uint16_t ubx_errorHeader1 = 0;
//! The number of bytes dropped in header2 state
uint16_t ubx_errorHeader2 = 0;
//! The number of messages dropped due to checksum error
uint16_t ubx_errorChecksum = 0;
//! The number of messages dropped due to unknown message class (or not taken into account)
uint16_t ubx_errorUnknownClass = 0;
//! The number of messages dropped due to unknown message id or wrong size read
uint16_t ubx_errorUnknownIdSize = 0;
//! The ID of the last message that was dropped due to unknown id or wrong size
uint8_t ubx_errorWrongId = 0;
//! The size of the last message that was dropped due to unknown id or wrong size
uint16_t ubx_errorWrongSize = 0;
//! The number of major errors that occured (should always be 0)
uint16_t ubx_errorMajor = 0;


//---------------------------------
// Public functions implementation
//---------------------------------

/*!
*	This function receives bytes from the communication
*	It runs a state machine which fills decodes the messages and put their content into structures
*
*	@param data The data byte that arrived through communication
*/
int ubx_Decode(uint8_t data)
{
    // the 
    uint8_t * temporaryMessageForSwaping;
    
    // return value
    int8_t ret = 0;
    
    // read header 1
    if (ubx_state == UBX_STATE_HEADER1)
    {
        if (data == UBX_HEADER1)
        {
            ubx_state = UBX_STATE_HEADER2;	
        }
        else
        {
            ++ubx_errorHeader1;
            ret = -1;
        }	
    }
    // read header 2
    else if (ubx_state == UBX_STATE_HEADER2)
    {
        if (data == UBX_HEADER2)
        {
            // set up next state
            ubx_currentMessageCkA = 0;
            ubx_currentMessageCkB = 0;
            ubx_state = UBX_STATE_CLASS;	
        }
        else if (data == UBX_HEADER1)
        {
            // stay in this state	
        }
        else
        {
            ++ubx_errorHeader2;
            ret = -1;
            // go back to first state
            ubx_state = UBX_STATE_HEADER1;	
        }	
    }
    // read the class of the message
    else if (ubx_state == UBX_STATE_CLASS)
    {
        // read class
        ubx_currentMessageClass = data;
        // checksum
        ubx_currentMessageCkA += data;
        ubx_currentMessageCkB += ubx_currentMessageCkA;
        // go to next state: id
        ubx_state = UBX_STATE_ID;
    }
    // read the id of the message
    else if (ubx_state == UBX_STATE_ID)
    {
        // read id
        ubx_currentMessageId = data;
        // checksum
        ubx_currentMessageCkA += data;
        ubx_currentMessageCkB += ubx_currentMessageCkA;
        // set up next state: length
        ubx_currentMessageSizeByte = 0;
        ubx_currentMessageSize = 0;
        ubx_state = UBX_STATE_LENGTH;
    }
    // read the size of the message
    else if (ubx_state == UBX_STATE_LENGTH)
    {
        ((uint8_t*)(&ubx_currentMessageSize))[ubx_currentMessageSizeByte++] = data;
        // checksum
        ubx_currentMessageCkA += data;
        ubx_currentMessageCkB += ubx_currentMessageCkA;
        
        // size read completely
        if (ubx_currentMessageSizeByte == 2)
        {
            
            // select structure to fill
            // NAV class messages
            if (ubx_currentMessageClass == UBX_CLASS_NAV)
            {
                // NAV-POSLLH
                if (ubx_currentMessageId == UBX_ID_NAV_POSLLH && ubx_currentMessageSize == UBX_SIZE_NAV_POSLLH)
                {
                    ubx_currentMessage = (uint8_t**)&ubx_currentPosllhMessage;
                    ubx_lastMessage = (uint8_t**)&ubx_lastPosllhMessage;
                    ubx_validMessage = &ubx_numberOfValidPosllhMessage;
//                    ubx_isMessageUpdated = &ubx_IsPosllhUpdated;
                    
                    ubx_isUnknown = false;
                    ubx_state = UBX_STATE_CONTENT;
                }
                // NAV-VELNED
                else if	(ubx_currentMessageId == UBX_ID_NAV_VELNED && ubx_currentMessageSize == UBX_SIZE_NAV_VELNED)
                {
                    ubx_currentMessage = (uint8_t**)&ubx_currentVelnedMessage;
                    ubx_lastMessage = (uint8_t**)&ubx_lastVelnedMessage;
                    ubx_validMessage = &ubx_numberOfValidVelnedMessage;
//                    ubx_isMessageUpdated = &ubx_IsVelnedUpdated;
                    
                    ubx_isUnknown = false;
                    ubx_state = UBX_STATE_CONTENT;
                }
                // NAV-STATUS
                else if (ubx_currentMessageId == UBX_ID_NAV_STATUS && ubx_currentMessageSize == UBX_SIZE_NAV_STATUS)
                {
                    ubx_currentMessage = (uint8_t**)&ubx_currentStatusMessage;
                    ubx_lastMessage = (uint8_t**)&ubx_lastStatusMessage;
                    ubx_validMessage = &ubx_numberOfValidStatusMessage;
//                    ubx_isMessageUpdated = &ubx_IsStatusUpdated;
                    
                    ubx_isUnknown = false;
                    ubx_state = UBX_STATE_CONTENT;
                }
                // NAV-SVINFO
                else if (ubx_currentMessageId == UBX_ID_NAV_SVINFO)
                {
                    ubx_currentMessage = (uint8_t**)&ubx_currentSVInfoMessage;
                    ubx_lastMessage = (uint8_t**)&ubx_lastSVInfoMessage;
                    ubx_validMessage = &ubx_numberOfValidSVInfoMessage;
//                    ubx_isMessageUpdated = &ubx_IsSVInfoUpdated;
                    
                    ubx_isUnknown = false;
                    ubx_state = UBX_STATE_CONTENT;
                }
                // NAV-SOL
                else if (ubx_currentMessageId == UBX_ID_NAV_SOL && ubx_currentMessageSize == UBX_SIZE_NAV_SOL)
                {
                    ubx_currentMessage = (uint8_t**)&ubx_currentSolMessage;
                    ubx_lastMessage = (uint8_t**)&ubx_lastSolMessage;
                    ubx_validMessage = &ubx_numberOfValidSolMessage;
//                    ubx_isMessageUpdated = &ubx_IsSolUpdated;
                    
                    ubx_isUnknown = false;
                    ubx_state = UBX_STATE_CONTENT;
                }
                // NAV-TIMEUTC
                else if (ubx_currentMessageId == UBX_ID_NAV_TIMEUTC && ubx_currentMessageSize == UBX_SIZE_NAV_TIMEUTC)
                {
                    ubx_currentMessage = (uint8_t**)&ubx_currentTimeUTCMessage;
                    ubx_lastMessage = (uint8_t**)&ubx_lastTimeUTCMessage;
                    ubx_validMessage = &ubx_numberOfValidTimeUTCMessage;
//                    ubx_isMessageUpdated = &ubx_IsSolUpdated;
                    
                    ubx_isUnknown = false;
                    ubx_state = UBX_STATE_CONTENT;
                }
                // not taken into account, so wait for next message
                else
                {
                    ++ubx_errorUnknownIdSize;
                    ubx_errorWrongId = ubx_currentMessageId;
                    ubx_errorWrongSize = ubx_currentMessageSize;
                    
                    ubx_isUnknown = true;
                    //ubx_state = UBX_STATE_HEADER1;
                    ubx_state = UBX_STATE_CONTENT;
                }		
            }	
            // not taken into account, so wait for next message
            else
            {
                ++ubx_errorUnknownClass;
                ubx_isUnknown = true;
                ret = -1;
                
                //ubx_state = UBX_STATE_HEADER1;
                ubx_state = UBX_STATE_CONTENT;
            }		
            
            // reset byte count anyway
            ubx_currentMessageCurrentByte = 0;
        }	
    }	
    // read the content of the message
    else if (ubx_state == UBX_STATE_CONTENT)
    {
        if(!ubx_isUnknown)
        {
            // read byte and put it in the structure
            (*ubx_currentMessage)[ubx_currentMessageCurrentByte++] = data;
            
            // checksum
            ubx_currentMessageCkA += data;
            ubx_currentMessageCkB += ubx_currentMessageCkA;
        
            // message is full, go to next state
            
        }
        else
            ubx_currentMessageCurrentByte++;
        if (ubx_currentMessageCurrentByte == ubx_currentMessageSize)
        {
            ubx_state = UBX_STATE_CHECKSUM_A;
        }
    }
    // read the checksum byte A
    else if (ubx_state == UBX_STATE_CHECKSUM_A)	
    {
        // read checksum A
        ubx_currentMessageCkARead = data;
        // go to last state: checksum B
        ubx_state = UBX_STATE_CHECKSUM_B;
    }	
    // read the checksum byte B
    else if (ubx_state == UBX_STATE_CHECKSUM_B)
    {
        // read checksum B
        ubx_currentMessageCkBRead = data;
        
        // checksum is correct ?
        if (ubx_currentMessageCkA == ubx_currentMessageCkARead && ubx_currentMessageCkB == ubx_currentMessageCkBRead)
        {
            if(!ubx_isUnknown)
            {
                // this type of message has been received successfully
                ++(*ubx_validMessage);
                ret = ubx_currentMessageId;
                //*ubx_isMessageUpdated = true;
                // message received, reset timeout
                //ubx_timeout = 0;
            
                // swap message buffers, old message is discarded and becomes incoming buffer, new message become valid message (=old)
                temporaryMessageForSwaping = *ubx_currentMessage;
                *ubx_currentMessage = *ubx_lastMessage;
                *ubx_lastMessage = temporaryMessageForSwaping;
            }
        }	
        // checksum error
        else
        {
            ++ubx_errorChecksum;
            ret = -1;
            // keep same buffers as data is corrupted
        }	
        // go back to state header1 for next message
        ubx_state = UBX_STATE_HEADER1;
    }	
    // major error, should never happen, the state is unknown !
    else
    {
        ++ubx_errorMajor;
        ret = -1;
        ubx_state = UBX_STATE_HEADER1;
    }
    return ret;
}	

/*!
*	This function calculates the number of time that it has been called since the last GPS message was received.
*	If this number goes higher than #UBX_TIMEOUT_CYCLES, then there is a timeout.
*
*	@return 1 if there is a timeout, 0 otherwise.
*/
/*
uint8_t ubx_CheckTimeout()
{
    if (ubx_timeout < UBX_TIMEOUT_CYCLES)
    {
        ++ubx_timeout;
    }	
    
    return (ubx_timeout == UBX_TIMEOUT_CYCLES);
}*/

/*!
*	This function returns if there is a timeout on the GPS.
*	It is passive and does not modifies the timeout value.
*/
/*
uint8_t ubx_GetTimeout()
{
    return (ubx_timeout == UBX_TIMEOUT_CYCLES);
}*/	

/*! 
*	This function returns a pointer to the last NAV-POSLLH message that was received
*	Warning: the values of the message must be read very quickly after the call to this function as buffer may be swapped in an interruption
*
*	@return A pointer to the last valid posllh message, or 0.
*/
//bool ubx_PosllhUpdated()
//{
//    return ubx_IsPosllhUpdated;
//}
//void ubx_PosllhClear()
//{
//    ubx_IsPosllhUpdated = false;
//}
ubx_MessagePosllh * ubx_GetPosllh()
{
    if (ubx_numberOfValidPosllhMessage)
        return ubx_lastPosllhMessage;
    else
        return 0;
}

/*! 
*	This function returns a pointer to the last NAV-VELNED message that was received
*	Warning: the values of the message must be read very quickly after the call to this function as buffer may be swapped in an interruption
*
*	@return A pointer to the last valid velned message, or 0.
*/
//bool ubx_VelnedUpdated()
//{
//    return ubx_IsVelnedUpdated;
//}
//void ubx_VelnedClear()
//{
//    ubx_IsVelnedUpdated = false;
//}
ubx_MessageVelned * ubx_GetVelned()
{
    if (ubx_numberOfValidVelnedMessage)
        return ubx_lastVelnedMessage;
    else
        return 0;
}

/*!
*	This function returns a pointer to the last NAV-STATUS message that was received
*	Warning: the values of the message must be read very quickly after the call to this function as buffer may be swapped in an interruption
*
*	@return A pointer to the last valid status message, or 0.
*/
//bool ubx_StatusUpdated()
//{
//    return ubx_IsStatusUpdated;
//}
//void ubx_StatusClear()
//{
//    ubx_IsStatusUpdated = false;
//}
ubx_MessageStatus * ubx_GetStatus()
{
    if (ubx_numberOfValidStatusMessage)
        return ubx_lastStatusMessage;
    else
        return 0;	
}

/*!
*	This function returns a pointer to the last NAV-SVINFO message that was received
*	Warning: the values of the message must be read very quickly after the call to this function as buffer may be swapped in an interruption
*
*	@return A pointer to the last valid status message, or 0.
*/
//bool ubx_SVInfoUpdated()
//{
//    return ubx_IsSVInfoUpdated;
//}
//void ubx_SVInfoClear()
//{
//    ubx_IsSVInfoUpdated = false;
//}
ubx_MessageSVInfo * ubx_GetSVInfo()
{
    if (ubx_numberOfValidSVInfoMessage)
        return ubx_lastSVInfoMessage;
    else
        return 0;	
}

/*!
*	This function returns a pointer to the last NAV-SOL message that was received
*	Warning: the values of the message must be read very quickly after the call to this function as buffer may be swapped in an interruption
*
*	@return A pointer to the last valid solution message, or 0.
*/
//bool ubx_SolUpdated()
//{
//    return ubx_IsSolUpdated;
//}
//void ubx_SolClear()
//{
//    ubx_IsSolUpdated = false;
//}
ubx_MessageSol    * ubx_GetSol()
{
    if (ubx_numberOfValidSolMessage)
        return ubx_lastSolMessage;
    else
        return 0;
}

/*!
*	This function returns a pointer to the last NAV-TIMEUTC message that was received
*	Warning: the values of the message must be read very quickly after the call to this function as buffer may be swapped in an interruption
*
*	@return A pointer to the last valid TimeUTC message, or 0.
*/
//bool ubx_timeUTCUpdated()
//{
//    return ubx_IsTimeUTCUpdated;
//}
//void ubx_timeUTCClear()
//{
//    ubx_IsTimeUTCUpdated = false;
//}
ubx_MessageTimeUTC * ubx_GetTimeUTC()
{
    if (ubx_numberOfValidTimeUTCMessage)
        return ubx_lastTimeUTCMessage;
    else
        return 0;
}
