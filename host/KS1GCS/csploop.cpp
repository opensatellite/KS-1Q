#include "csploop.h"
#include <QFile>
#include <QDateTime>
#include <QDebug>
#include "dm_gcs.h"
#include "dm_data_obc.h"

CspLoop::CspLoop(csp_socket_t *sock, QObject *parent) :
    QObject(parent), m_sock(sock), run(false)
{

}

void CspLoop::doWork()
{
    run = true;
    cam_sock = csp_socket(CSP_SO_NONE);
    csp_listen(cam_sock, 200);
    csp_bind(cam_sock, dm_obc_cam);
    csp_conn_t *conn = 0;
    QFile imgbinary(QString("imgbin_") + QDateTime::currentDateTimeUtc().toString("yymmdd_hhmmss") + QString(".raw"));
    imgbinary.open(QIODevice::WriteOnly | QIODevice::Append);

    while(run) {

        if(!conn)
            conn = csp_accept(cam_sock, 1);

        if(conn) {
            csp_packet_t *cam_packet = csp_read(conn, 1);
            if(cam_packet) {
            imgbinary.write((const char *)cam_packet->data, cam_packet->length);
            qDebug() << "CAM data" << cam_packet->length;
            }
        }

        csp_packet_t *packet = csp_recvfrom(m_sock, 1);
        if(packet == NULL) continue;

        emit cspReadyRead(packet);
        csp_log_packet("CSP: S %u, D %u, Dp %u, Sp %u, Pr %u, Fl 0x%02X, Sz %"PRIu16"",
                   packet->id.src, packet->id.dst, packet->id.dport,
                   packet->id.sport, packet->id.pri, packet->id.flags, packet->length);
    }

    imgbinary.close();
}

void CspLoop::stop()
{
    run = false;
}
