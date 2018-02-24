/*! \file packet.h
    \brief Transmit or receive data packet through serial port

    This file handles serial data packing and unpacking logic
*/

#ifndef __PACKET_H__
#define __PACKET_H__

#include "datatypes.h"

/*
    Telemetry Packet Format
    | Header | Type | Payload Size | Payload | Checksum  |
    | A5 7C  | type | LO HI        | Data    | CK_A CK_B |
             |<-----------Checksum---------->|
*/

#define PACKET_HEAD1    0xa5
#define PACKET_HEAD2    0x7c

typedef struct {
    int fd;
    //os_mutex_t lock;
    
    uint8_t tx_ck_a;
    uint8_t tx_ck_b;
    
    uint8_t rx_type;
    int rx_count;
    int rx_size;
    uint8_t rx_ck_a;
    uint8_t rx_ck_b;
}packet_session_t;

/*! \brief Initialize a session using \a fd
    \param session Session to init
    \param fd File descriptor occupied by \a session
    \return 0 = ok, <0 when error occurred
    
    This function initialize all variables in \a session. Then set file 
    \a session->fd as communication port for \a session and open it.
    Return 0 = ok, or return a error number when failed.
*/
int  packet_init(packet_session_t *session, int fd);

/*! \brief Deinitialize(shutdown) a session
    \param session Session to deinit

    Close the file descriptor \a session->fd and free the mutex lock \a session->lock
*/
void pakcet_deinit(packet_session_t *session);

void packet_tx_head(packet_session_t *session,uint8_t type,int size);
void packet_tx(packet_session_t *session,void* ptr,int len);
void packet_tx_tail(packet_session_t *session);

/*! \brief Check \a session and return type and length of next packet 
    \param session Session to check
    \param type Pointer to variable which keeps the type of next packet 
    \return Size of next packet, -1 = no packet received
*/
int  packet_rx_check(packet_session_t *session, uint8_t *type);

/*! \brief Query type of next packet
    \param session Session to query
    \return Type of next packet, -1 = no packet received
*/
int  packet_rx_type(packet_session_t *session);

/*! \brief Check \a session and return type and length of next packet 
    \param session Session to check
    \param buffer Pointer to packet content buffer
    \return Size of next packet, -1 = checksum failed
*/
int  packet_rx(packet_session_t *session,uint8_t *buffer);
                    
#endif