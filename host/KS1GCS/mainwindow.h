#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>
#include <QThread>
#include "csploop.h"
#include <csp/csp.h>
#include <csp/interfaces/csp_if_kiss.h>

namespace Ui {
class MainWindow;
}

//never start two instance for mainwindow
//because csplink is static
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Ui::MainWindow *ui;

    static QTcpSocket *csplink;
    static void kissPutc(char buf);
    static void kissDiscard(char c, void *pxTaskWoken);

protected:

private slots:
    void tcpReadBytes();
    void tcpStatusChanged(QAbstractSocket::SocketState);


    void on_tcpconn_clicked();

    void on_tcpdisconn_clicked();

    void on_obcon_clicked();

    void on_camon_clicked();

    void on_pingstart_clicked();

    void pingAction();

    void on_pingstop_clicked();

    void cspReadPacket(csp_packet_t *packet);
    void parse_eps_bcast(uint8_t* data, int size);
    void parse_ttc_bcast(uint8_t* data, int size);
    void parse_obc_bcast(uint8_t* data, int size);

    void on_csptest_clicked();

    void on_camoff_clicked();

    void on_obcoff_clicked();

    void on_heateron_clicked();

    void on_heateroff_clicked();

    void on_epsreboot_clicked();

    void on_ledsubmit_clicked();

    void on_fastbeaconset_clicked();

    void on_fastbeaconclear_clicked();

    void on_ttcreboot_clicked();


    void on_camshoot_clicked();

    void on_camstop_clicked();

    void on_imustart_clicked();

    void on_imustop_clicked();

    void on_inject_clicked();

private:
    QTcpServer *telnet;
    csp_socket_t *cspsock;
    csp_iface_t csp_if_kiss;
    csp_kiss_handle_t csp_kiss_driver;
    QTimer ping_timer;
    QThread cspThread;
    CspLoop *cspLoopWorker;
};

#endif // MAINWINDOW_H
