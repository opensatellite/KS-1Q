#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <csp/csp.h>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include "dm_gcs.h"
#include "dm_data_eps.h"
#include "dm_data_obc.h"
#include "dm_data_ttc.h"

QTcpSocket *MainWindow::csplink = NULL;

void print_packet_bytes(uint8_t *data, int len)
{
    std::cout << "Data: " << std::endl;
    while(len--) std::cout << std::hex << (int)*data++ << " " << std::dec;
    std::cout << std::endl;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    telnet(0),
    cspsock(0)
{
    ui->setupUi(this);
    ui->logo->setPixmap(QPixmap::fromImage(QImage(":/ks1q.png").scaledToWidth(500)));

    csplink = new QTcpSocket(this);
    connect(csplink, SIGNAL(readyRead()), this, SLOT(tcpReadBytes()));
    connect(csplink, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(tcpStatusChanged(QAbstractSocket::SocketState)));

    connect(&ping_timer, SIGNAL(timeout()), this, SLOT(pingAction()));
    ping_timer.stop();

    //csp_debug_toggle_level(CSP_ERROR);
    //csp_debug_toggle_level(CSP_WARN	);
    csp_debug_toggle_level(CSP_INFO);
    //csp_debug_toggle_level(CSP_BUFFER);
    csp_debug_toggle_level(CSP_PACKET);
    csp_debug_toggle_level(CSP_PROTOCOL	);
    //csp_debug_toggle_level(CSP_LOCK);

    csp_buffer_init(1000, 300);
    csp_init(TNC_ADDR);

    memset(&csp_kiss_driver, 0, sizeof(csp_kiss_driver));
    csp_kiss_init(&csp_if_kiss, &csp_kiss_driver, MainWindow::kissPutc, MainWindow::kissDiscard, "Radio");
    csp_route_set(EPS_ADDR, &csp_if_kiss, CSP_NODE_MAC);
    csp_route_set(OBC_ADDR, &csp_if_kiss, CSP_NODE_MAC);
    csp_route_set(TTC_ADDR, &csp_if_kiss, CSP_NODE_MAC);
    csp_route_start_task(0, -1);

    csp_conn_print_table();
    csp_route_print_table();
    csp_route_print_interfaces();

    cspsock = csp_socket(CSP_SO_CONN_LESS);
    csp_bind(cspsock, CSP_ANY);

    cspLoopWorker = new CspLoop(cspsock);
    cspLoopWorker->moveToThread(&cspThread);
    connect(&cspThread, SIGNAL(finished()), cspLoopWorker, SLOT(deleteLater()));
    connect(&cspThread, SIGNAL(started()), cspLoopWorker, SLOT(doWork()));
    connect(cspLoopWorker, SIGNAL(cspReadyRead(csp_packet_t*)), this, SLOT(cspReadPacket(csp_packet_t*)));
    cspThread.start();
}

MainWindow::~MainWindow()
{
    cspLoopWorker->stop();
    cspThread.quit();
    cspThread.wait(5000);
    csplink->close();
    delete csplink;
    delete ui;
}

void MainWindow::tcpReadBytes()
{
    if(!csplink) return;
    if(!csplink->isReadable()) return;
    QByteArray buf = csplink->readAll();
    bool dummy = false;
    uint8_t *bytes_in = (uint8_t*)buf.data();
    if(!bytes_in) return;
    int nbytes = buf.size();
    if(nbytes == 0) return;
    csp_kiss_rx(&csp_if_kiss, bytes_in, nbytes, &dummy);
    Q_UNUSED(dummy);
}

void MainWindow::tcpStatusChanged(QAbstractSocket::SocketState state)
{
    QString msg;
    switch(state) {
        case QAbstractSocket::UnconnectedState:
        msg = trUtf8("断开");
        break;
        case QAbstractSocket::ConnectingState:
        msg = trUtf8("正在连接");
        break;
        case QAbstractSocket::ConnectedState:
        msg = trUtf8("已连接");
        break;
        case QAbstractSocket::ClosingState:
        msg = trUtf8("正在关闭");
        break;
    default:
        msg = trUtf8("未知");
    }
    ui->tcpstatus->setText(msg);
}

void MainWindow::kissPutc(char buf)
{
    if(!csplink->isWritable()) return;
    csplink->write(&buf, 1);
}

void MainWindow::kissDiscard(char c, void *pxTaskWoken)
{
    Q_UNUSED(pxTaskWoken);
    std::cout << c;
}


void MainWindow::on_tcpconn_clicked()
{
    csplink->connectToHost(QHostAddress(ui->tcphost->text()), ui->tcpport->value());
}

void MainWindow::on_tcpdisconn_clicked()
{
    csplink->disconnectFromHost();
}

void MainWindow::on_obcon_clicked()
{
    csp_packet_t *packet = (csp_packet_t*)csp_buffer_get(10);
    struct dm_op_s *op = (struct dm_op_s *)&(packet->data);
    op->op = DM_W_VAR;
    struct dm_eps_var_s var;
    op->offset = (long)&(var.deploy) - (long)&var;
    op->size = sizeof(var.deploy);
    op->data[0] = 1;
    packet->length = sizeof(struct dm_op_s) + op->size;
    print_packet_bytes(packet->data, packet->length);
    csp_sendto(CSP_PRIO_HIGH, EPS_ADDR, dm_service_port, dm_service_port, 0, packet, 1000);
}

void MainWindow::on_camon_clicked()
{
    csp_packet_t *packet = (csp_packet_t*)csp_buffer_get(10);
    struct dm_op_s *op = (struct dm_op_s *)&(packet->data);
    op->op = DM_W_VAR;
    struct dm_eps_var_s var;
    op->offset = (long)&(var.pwrcam) - (long)&var;
    op->size = sizeof(var.pwrcam);
    op->data[0] = (quint8)ui->pwrcamset->value();
    packet->length = sizeof(struct dm_op_s) + op->size;
    print_packet_bytes(packet->data, packet->length);
    csp_sendto(CSP_PRIO_HIGH, EPS_ADDR, dm_service_port, dm_service_port, 0, packet, 1000);
}

void MainWindow::on_pingstart_clicked()
{
    ping_timer.setInterval(2000);
    ping_timer.start();
    ui->pingstart->setStyleSheet("border: 2px solid red;");
}

void MainWindow::pingAction()
{
    csp_ping(EPS_ADDR, 1000, 1, 0);
}

void MainWindow::on_pingstop_clicked()
{
    ping_timer.stop();
    ui->pingstart->setStyleSheet("");
}

void MainWindow::cspReadPacket(csp_packet_t *packet)
{
    //qDebug() << "cspReadPacket" << packet;

    if(packet->id.src == EPS_ADDR && packet->id.dport == dm_bcast_port) {
        parse_eps_bcast(packet->data, packet->length);
    } else if(packet->id.src == TTC_ADDR && packet->id.dport == dm_bcast_port) {
        parse_ttc_bcast(packet->data, packet->length);
    } else if(packet->id.src == OBC_ADDR && packet->id.dport == dm_bcast_port) {
        parse_obc_bcast(packet->data, packet->length);
    }

    csp_buffer_free(packet);
}

void MainWindow::parse_eps_bcast(uint8_t *data, int size)
{
    struct dm_eps_var_s eps_var;
    struct dm_eps_cfg_s eps_cfg;
    if(size != dm_eps_var_bcast_size + dm_eps_cfg_bcast_size) {
        qDebug() << "eps bcast size not match";
        return;
    }
    memcpy(&eps_var, data, dm_eps_var_bcast_size);
    memcpy(&eps_cfg, data + dm_eps_var_bcast_size, dm_eps_cfg_bcast_size);

    ui->solar1i->setText(QString("%1").arg(eps_var.solar1i));
    ui->solar2i->setText(QString("%1").arg(eps_var.solar2i));
    ui->accel->setText(QString("%1").arg(eps_var.accel));
    ui->heater->setText(QString("%1").arg(eps_var.heater));
    ui->pwrobc->setText(QString("%1").arg(eps_var.deploy));
    ui->pwrcam->setText(QString("%1").arg(eps_var.pwrcam));
    ui->batvh->setText(QString("%1").arg(eps_var.batvh));
    ui->batvl->setText(QString("%1").arg(eps_var.batvl));
    ui->ibat->setText(QString("%1").arg(eps_var.ibat));
    ui->ipsb->setText(QString("%1").arg(eps_var.ipsb));
    ui->iheater->setText(QString("%1").arg(eps_var.iheater));
    ui->ibus->setText(QString("%1").arg(eps_var.ibus));
    ui->iled->setText(QString("%1").arg(eps_var.iled));
    ui->icam->setText(QString("%1").arg(eps_var.icam));
    ui->battemp->setText(QString("%1").arg(eps_var.battemp));
    ui->camtemp->setText(QString("%1").arg(eps_var.camtemp));
    ui->temp1->setText(QString("%1").arg(eps_var.temp1));
    ui->temp2->setText(QString("%1").arg(eps_var.temp2));
    ui->temp3->setText(QString("%1").arg(eps_var.temp3));
    ui->temp4->setText(QString("%1").arg(eps_var.temp4));
    ui->radraw->setText(QString("%1").arg(eps_var.radraw));
    //ui->datetime->setText(QString("%1").arg(eps_var.ibus));
    ui->suna->setText(QString("%1").arg(eps_var.suna));
    ui->sunb->setText(QString("%1").arg(eps_var.sunb));
    ui->sunc->setText(QString("%1").arg(eps_var.sunc));
    ui->sund->setText(QString("%1").arg(eps_var.sund));
    ui->ebatvl->setText(QString("%1").arg(eps_var.ebatvl));
    ui->epsresets->setText(QString("%1").arg(eps_var.resets));
    ui->epswdt->setText(QString("%1").arg(eps_var.wdt));
    ui->epsfirstrun->setText(QString("%1").arg(eps_cfg.eps_run_wait));
    ui->epsobcen->setText(QString("%1").arg(eps_cfg.obc_run_flag));
    ui->epsheateren->setText(QString("%1").arg(eps_cfg.heat_valid));
}

void MainWindow::parse_ttc_bcast(uint8_t *data, int size)
{
    struct dm_ttc_var_s ttc_var;
    struct dm_ttc_cfg_s ttc_cfg;
    if(size != dm_ttc_var_bcast_size + dm_ttc_cfg_bcast_size) {
        qDebug() << "ttc bcast size not match";
        return;
    }
    memcpy(&ttc_var, data, dm_ttc_var_bcast_size);
    memcpy(&ttc_cfg, data + dm_ttc_var_bcast_size, dm_ttc_cfg_bcast_size);

    ui->txpkts->setText(QString("%1").arg(ttc_var.txpkts));
    ui->rxpkts->setText(QString("%1").arg(ttc_var.rxpkts));
    ui->rxerrs->setText(QString("%1").arg(ttc_var.rxerrs));
    ui->canerrs->setText(QString("%1").arg(ttc_var.canerrs));
    ui->mcutemp->setText(QString("%1").arg(ttc_var.mcutemp));
    ui->patemp->setText(QString("%1").arg(ttc_var.patemp));
    ui->ittc->setText(QString("%1").arg(ttc_var.ittc));
    ui->ipa->setText(QString("%1").arg(ttc_var.ipa));
    ui->ttcresets->setText(QString("%1").arg(ttc_var.resets));
    ui->rssi->setText(QString("%1").arg(ttc_var.rssi));
    ui->binterval->setText(QString("%1").arg(ttc_var.binterval));
    ui->btimeout->setText(QString("%1").arg(ttc_var.btimeout));

}

void MainWindow::parse_obc_bcast(uint8_t *data, int size)
{
    struct dm_obc_var_s obc_var;
    struct dm_obc_cfg_s obc_cfg;
    if(size != dm_obc_var_bcast_size + dm_obc_cfg_bcast_size) {
        qDebug() << "obc bcast size not match";
        return;
    }
    memcpy(&obc_var, data, dm_obc_var_bcast_size);
    memcpy(&obc_cfg, data + dm_obc_var_bcast_size, dm_obc_cfg_bcast_size);

    ui->magx->setText(QString("%1").arg(obc_var.magx));
    ui->magy->setText(QString("%1").arg(obc_var.magy));
    ui->magz->setText(QString("%1").arg(obc_var.magz));
    ui->magtemp->setText(QString("%1").arg(obc_var.magtemp));
    ui->obcresets->setText(QString("%1").arg(obc_var.resets));
    ui->obcwdt->setText(QString("%1").arg(obc_var.wdt));
    ui->obcvin->setText(QString("%1").arg(obc_var.vin));
    ui->obccanerrs->setText(QString("%1").arg(obc_var.canerrs));
    ui->memstatus->setText(QString("%1").arg(obc_var.memstatus));
    ui->formatdisk->setText(QString("%1").arg(obc_var.formatdisk));
    ui->memavail->setText(QString("%1").arg(obc_var.memavail));
    ui->lastfile->setText(QString("%1").arg(obc_var.lastfile));
    ui->pwrimu->setText(QString("%1").arg(obc_var.pwrimu));
    ui->imuinterval->setText(QString("%1").arg(obc_var.imuinterval));
    ui->gyrox->setText(QString("%1").arg(obc_var.gyrox));
    ui->gyroy->setText(QString("%1").arg(obc_var.gyroy));
    ui->gyroz->setText(QString("%1").arg(obc_var.gyroz));
    ui->accx->setText(QString("%1").arg(obc_var.accx));
    ui->accy->setText(QString("%1").arg(obc_var.accy));
    ui->accz->setText(QString("%1").arg(obc_var.accz));
    ui->imutemp->setText(QString("%1").arg(obc_var.imutemp));
    ui->pressure->setText(QString("%1").arg(obc_var.baro));
    ui->barotemp->setText(QString("%1").arg(obc_var.barotemp));
}

void MainWindow::on_csptest_clicked()
{
    quint8 testdata[] = {
        0xc0,0x00,0x84,0x92,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0,0,0xb0,0x20,0xcd,0x0c,0,0,0x42,0x6b,0,0,0,0,0x4e,0x25,
        0,0,0x28,0x28,0,0,0,0,0x87,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0xff,0xff,0xff,
        0xff,0x01,0x73,0x4c,0xfa,0x0b,0xd1,0x92,0x74,0xc0
    };
    if(ui->mode_groundtest->isChecked()) {
        //only used in ground test mode
        csp_kiss_rx(&csp_if_kiss, testdata, sizeof(testdata), NULL);
    }
}

void MainWindow::on_camoff_clicked()
{
    csp_packet_t *packet = (csp_packet_t*)csp_buffer_get(10);
    struct dm_op_s *op = (struct dm_op_s *)&(packet->data);
    op->op = DM_W_VAR;
    struct dm_eps_var_s var;
    op->offset = (long)&(var.pwrcam) - (long)&var;
    op->size = sizeof(var.pwrcam);
    op->data[0] = 0;
    packet->length = sizeof(struct dm_op_s) + op->size;
    print_packet_bytes(packet->data, packet->length);
    csp_sendto(CSP_PRIO_HIGH, EPS_ADDR, dm_service_port, dm_service_port, 0, packet, 1000);
}

void MainWindow::on_obcoff_clicked()
{
    csp_packet_t *packet = (csp_packet_t*)csp_buffer_get(10);
    struct dm_op_s *op = (struct dm_op_s *)&(packet->data);
    op->op = DM_W_VAR;
    struct dm_eps_var_s var;
    op->offset = (long)&(var.deploy) - (long)&var;
    op->size = sizeof(var.deploy);
    op->data[0] = 0;
    packet->length = sizeof(struct dm_op_s) + op->size;
    print_packet_bytes(packet->data, packet->length);
    csp_sendto(CSP_PRIO_HIGH, EPS_ADDR, dm_service_port, dm_service_port, 0, packet, 1000);
}

void MainWindow::on_heateron_clicked()
{
    csp_packet_t *packet = (csp_packet_t*)csp_buffer_get(10);
    struct dm_op_s *op = (struct dm_op_s *)&(packet->data);
    op->op = DM_W_VAR;
    struct dm_eps_var_s var;
    op->offset = (long)&(var.heater) - (long)&var;  //---------------var name
    op->size = sizeof(var.heater);  //------------var size
    op->data[0] = 1;    //------------var value
    packet->length = sizeof(struct dm_op_s) + op->size;
    print_packet_bytes(packet->data, packet->length);
    csp_sendto(CSP_PRIO_HIGH, EPS_ADDR, dm_service_port, dm_service_port, 0, packet, 1000);
}

void MainWindow::on_heateroff_clicked()
{
    csp_packet_t *packet = (csp_packet_t*)csp_buffer_get(10);
    struct dm_op_s *op = (struct dm_op_s *)&(packet->data);
    op->op = DM_W_VAR;
    struct dm_eps_var_s var;
    op->offset = (long)&(var.heater) - (long)&var;  //---------------var name
    op->size = sizeof(var.heater);  //------------var size
    op->data[0] = 0;    //------------var value
    packet->length = sizeof(struct dm_op_s) + op->size;
    print_packet_bytes(packet->data, packet->length);
    csp_sendto(CSP_PRIO_HIGH, EPS_ADDR, dm_service_port, dm_service_port, 0, packet, 1000);
}

void MainWindow::on_epsreboot_clicked()
{
    csp_reboot(EPS_ADDR);
}

void MainWindow::on_ledsubmit_clicked()
{
    QString hextext = ui->leddata->toPlainText();
    hextext = hextext.toLower();
    QString hextext2;
    for(int i = 0; i < hextext.size(); i++) {
        QChar c = hextext.at(i);
        if((c >= 0 && c <= 9) || (c >= 'a' || c <= 'f'))
            hextext2.push_back(c);
    }
    QByteArray leddata = QByteArray::fromHex(hextext2.toLatin1());
    if(leddata.size() > 256) leddata.resize(256 - 8);
    csp_packet_t *packet = (csp_packet_t*)csp_buffer_get(leddata.size());
    memcpy(&packet->data, leddata.data(), leddata.size());
    packet->length = leddata.size();
    std::cout << "LED: ";
    print_packet_bytes(packet->data, packet->length);
    csp_sendto(CSP_PRIO_LOW, EPS_ADDR, eps_led_port, eps_led_port, 0, packet, 1000);
}

void MainWindow::on_fastbeaconset_clicked()
{
    struct dm_ttc_var_s var;

    var.binterval = ui->bintervalset->value();
    var.btimeout = ui->btimeoutset->value();
    int varsize = sizeof(var.binterval) + sizeof(var.btimeout);
    int pktsize = sizeof(struct dm_op_s) + varsize;

    csp_packet_t *packet = (csp_packet_t*)csp_buffer_get(pktsize);
    struct dm_op_s *op = (struct dm_op_s *)&(packet->data);
    op->op = DM_W_VAR;
    op->offset = (long)&(var.binterval) - (long)&var; //---------------var name
    op->size = varsize; //------------var size
    memcpy(op->data, &var.binterval, varsize);

    packet->length = pktsize;
    print_packet_bytes(packet->data, packet->length);
    csp_sendto(CSP_PRIO_HIGH, TTC_ADDR, dm_service_port, dm_service_port, 0, packet, 1000);
}

void MainWindow::on_fastbeaconclear_clicked()
{
    struct dm_ttc_var_s var;

    var.binterval = 10000;
    var.btimeout = 0;
    int varsize = sizeof(var.binterval) + sizeof(var.btimeout);
    int pktsize = sizeof(struct dm_op_s) + varsize;

    csp_packet_t *packet = (csp_packet_t*)csp_buffer_get(pktsize);
    struct dm_op_s *op = (struct dm_op_s *)&(packet->data);
    op->op = DM_W_VAR;
    op->offset = (long)&(var.binterval) - (long)&var; //---------------var name
    op->size = varsize; //------------var size
    memcpy(op->data, &var.binterval, varsize);

    packet->length = pktsize;
    print_packet_bytes(packet->data, packet->length);
    csp_sendto(CSP_PRIO_HIGH, TTC_ADDR, dm_service_port, dm_service_port, 0, packet, 1000);
}

void MainWindow::on_ttcreboot_clicked()
{
    csp_reboot(TTC_ADDR);
}

void MainWindow::on_camshoot_clicked()
{
    struct dm_obc_var_s var;

    int mode = 0;
    if(ui->camsmall->isChecked()) mode = 1;
    if(ui->cammedium->isChecked()) mode = 2;
    if(ui->camlarge->isChecked()) mode = 3;
    if(ui->camtimedshot->isChecked()) mode = 4;
    if(ui->camattshot->isChecked()) mode = 5;
    if(ui->camvideo->isChecked()) mode = 6;
    if(ui->camnow->isChecked()) mode = 7;

    var.cammode = mode;
    var.camcount = ui->camcount->value();
    var.camdelay = ui->camdelay->value();
    var.caminterval = ui->caminterval->value();

    int varsize = sizeof(var.cammode) + sizeof(var.camcount) + sizeof(var.camdelay) + sizeof(var.caminterval);
    int pktsize = sizeof(struct dm_op_s) + varsize;

    csp_packet_t *packet = (csp_packet_t*)csp_buffer_get(pktsize);

    struct dm_op_s *op = (struct dm_op_s *)&(packet->data);
    op->op = DM_W_VAR;
    op->offset = (long)&(var.cammode) - (long)&var; //---------------var name
    op->size = varsize; //------------var size
    memcpy(op->data, &var.cammode, varsize);

    packet->length = pktsize;
    print_packet_bytes(packet->data, packet->length);
    csp_sendto(CSP_PRIO_HIGH, OBC_ADDR, dm_service_port, dm_service_port, 0, packet, 1000);
}

void MainWindow::on_camstop_clicked()
{
    struct dm_obc_var_s var;
    var.cammode = 0;

    int varsize = sizeof(var.cammode);
    int pktsize = sizeof(struct dm_op_s) + varsize;

    csp_packet_t *packet = (csp_packet_t*)csp_buffer_get(pktsize);

    struct dm_op_s *op = (struct dm_op_s *)&(packet->data);
    op->op = DM_W_VAR;
    op->offset = (long)&(var.cammode) - (long)&var; //---------------var name
    op->size = varsize; //------------var size
    memcpy(op->data, &var.cammode, varsize);

    packet->length = pktsize;
    print_packet_bytes(packet->data, packet->length);
    csp_sendto(CSP_PRIO_HIGH, OBC_ADDR, dm_service_port, dm_service_port, 0, packet, 1000);
}

void MainWindow::on_imustart_clicked()
{
    struct dm_obc_var_s var;

    var.pwrimu = ui->pwrimuset->value();
    var.imuinterval = ui->imuintervalset->value();

    int varsize = sizeof(var.pwrimu) + sizeof(var.imuinterval);
    int pktsize = sizeof(struct dm_op_s) + varsize;

    csp_packet_t *packet = (csp_packet_t*)csp_buffer_get(pktsize);

    struct dm_op_s *op = (struct dm_op_s *)&(packet->data);
    op->op = DM_W_VAR;
    op->offset = (long)&(var.pwrimu) - (long)&var; //---------------var name
    op->size = varsize; //------------var size
    memcpy(op->data, &var.pwrimu, varsize);

    packet->length = pktsize;
    print_packet_bytes(packet->data, packet->length);
    csp_sendto(CSP_PRIO_HIGH, OBC_ADDR, dm_service_port, dm_service_port, 0, packet, 1000);
}

void MainWindow::on_imustop_clicked()
{
    struct dm_obc_var_s var;

    var.pwrimu = 0;

    int varsize = sizeof(var.pwrimu);
    int pktsize = sizeof(struct dm_op_s) + varsize;

    csp_packet_t *packet = (csp_packet_t*)csp_buffer_get(pktsize);

    struct dm_op_s *op = (struct dm_op_s *)&(packet->data);
    op->op = DM_W_VAR;
    op->offset = (long)&(var.pwrimu) - (long)&var; //---------------var name
    op->size = varsize; //------------var size
    memcpy(op->data, &var.pwrimu, varsize);

    packet->length = pktsize;
    print_packet_bytes(packet->data, packet->length);
    csp_sendto(CSP_PRIO_HIGH, OBC_ADDR, dm_service_port, dm_service_port, 0, packet, 1000);
}

void MainWindow::on_inject_clicked()
{
    QString hextext = ui->binaryEditor->toPlainText();
    hextext = hextext.toLower();
    QString hextext2;
    for(int i = 0; i < hextext.size(); i++) {
        QChar c = hextext.at(i);
        if((c >= 0 && c <= 9) || (c >= 'a' || c <= 'f'))
            hextext2.push_back(c);
    }
    QByteArray injectdata = QByteArray::fromHex(hextext2.toLatin1());
    //print_packet_bytes((uint8_t*)leddata.data(), leddata.length());

    if(ui->mode_groundtest->isChecked()) {
        csp_kiss_rx(&csp_if_kiss, (uint8_t*)injectdata.data(), injectdata.length(), NULL);
    }
}
