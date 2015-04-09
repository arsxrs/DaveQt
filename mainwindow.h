#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <plcqtlib.h>
#include <QString>
#include <QMessageBox>
#include <QBitArray>
#include <QByteArray>
#include <QStatusBar>
#include <QProgressBar>
#include <QTimer>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkInterface>

#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include "mythread.h"
#include "item.h"
#include <QKeyEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QPoint>
#include <QModelIndex>
#include <QSignalMapper>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QBitArray bit;
    QString str;
    QString mess;
    Plcqtlib *conplc;
    QCustomPlot *customPlot;
    QVector <double>  x;
    QVector <double>  y;
private:
    Ui::MainWindow *ui;
    QString ip ,ipaddress;
    int port,statotcp,mpi,rack,slot;
    bool stato;
    QProgressBar *progressBar;
    QNetworkInterface *qnetint;
    QHostAddress *adr;
    QTimer *timer;
    QLabel *text;
    bool plcok;
    int value;

    int plotcount;

    QString demoName;
    QTimer dataTimer;

    MyThread *thr;


    QMenu tableContMenuType;
    // QMenu tableContMenuType;

    //   QCPItemTracer *itemDemoPhaseTracer;

    //Row element
    int rowCount;
    Item *itm[100];

    QSignalMapper *signalMapper;


public slots:





    void ConnectPlc();
    void Mess();
    void PlcOk(bool stato);

    //    void StopPlc();
    //    void RunPlc();
    //    void ReadDb();
    //    void WriteDb();
    void TimEvent();
    //    void SetBit();
    //    void ResetBit();
    //    void SetByte();


signals:
    void NetPlcStato(bool);

public slots:
    void Slot(void);
    void keyPressEvent(QKeyEvent *event) ;
    void slotSectionClicked(int);

private slots:
    void on_pbAddRow_clicked();
    void on_tableWidget_customContextMenuRequested(const QPoint &pos);
    void tableContextMenuTypeClick(const QString & text);







    void on_tableWidget_clicked(const QModelIndex &index);
    void on_tableWidget_cellChanged(int row, int column);
};

#endif // MAINWINDOW_H
