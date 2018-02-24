#include "packet.h"
#include "os.h"

int  packet_init(packet_session_t *session, int fd)
{
    memset(session,0,sizeof(packet_session_t));
    session->fd = fd;
    //session->lock = os_mutex_create();
    return uart_open(session->fd,O_RDWR);
}

void pakcet_deinit(packet_session_t *session)
{
    uart_close(session->fd);
    //os_mutex_delete(session->lock);
}

void packet_tx(packet_session_t *session,void* ptr,int len)
{
    uart_write(session->fd,ptr,len);
    for(size_t i=0;i<len;i++)
    {
        session->tx_ck_a += ((uint8_t*)ptr)[i];
        session->tx_ck_b += session->tx_ck_a;
    }
}

void packet_tx_head(packet_session_t *session,uint8_t type,int size)
{
    uint8_t head[] = {PACKET_HEAD1,PACKET_HEAD2};
    //os_mutex_lock(session->lock,timeout);
    packet_tx(session,head,2);
    session->tx_ck_a = 0;
    session->tx_ck_b = 0;
    packet_tx(session,&type,1);
    packet_tx(session,&size,2);//little endian
}

void packet_tx_tail(packet_session_t *session)
{
    packet_tx(session,&session->tx_ck_a,1);
    packet_tx(session,&session->tx_ck_b,1);
    //os_mutex_unlock(session->lock);
}
