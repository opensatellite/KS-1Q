#ifndef CSPLOOP_H
#define CSPLOOP_H

#include <QObject>
#include <csp/csp.h>

class CspLoop : public QObject
{
    Q_OBJECT
public:
    explicit CspLoop(csp_socket_t *sock, QObject *parent = 0);

public slots:
    void doWork();
    void stop();

signals:
    void cspReadyRead(csp_packet_t*);

public slots:

private:
    csp_socket_t *m_sock;
    csp_socket_t *cam_sock;
    bool run;
};

#endif // CSPLOOP_H
