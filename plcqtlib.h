//  project by Vincenzo D'Agostino (vindagos@gmail.com)


#ifndef PLCQTLIB_H
#define PLCQTLIB_H

#include <QtCore>
#include <QString>
#include <QObject>
#include <QByteArray>
#include <QBitArray>
#include <QList>
#include <libnodave/nodave.h>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>


#define WORD 2
#define DWORD 4
#define BYTE 1

class  Plcqtlib : public QObject
{

    Q_OBJECT

public:
    Plcqtlib();
    ~Plcqtlib();
    daveConnection * getConnectionPlc()const;
    int setTcp(QString ip , int port);
    int setPlc(QString plctype, int mpi, int rack, int slot);
    int stopPlc()const;
    int runPlc()const;
    int disconnectPlc() const;

    // Util
    QBitArray intToBitArray(int num , int size);
    int BitArrayToInt(QBitArray toint, int nbit);
    QString getAddress()const;
    QList <QString> getDaveBlockList();
    int getPort()const;
    int getSocketDescriptor()const;
    virtual QString  getError()const;

    //Read data
    int getDbInt(int numdb,int areastart);
    double getDbDint(int numdb,int areastart);
    float getDbReal(int numdb,int areastart);
    int getDbWord(int numdb,int areastart);
    double getDbDword(int numdb,int areastart);
    int getDbByte(int numdb, int areastart);
    QBitArray getBitArray(QString area ,int numdb, int areastart, int arealen);

    //Write data
    void setDbInt(int numdb,int areastart ,int datodb);
    void setDbDint(int numdb,int areastart ,double datodb);
    void setDbReal(int numdb,int areastart, float datodb);
    void setDbWord(int numdb,int areastart,int datodb);
    void setDbDword(int numdb,int areastart,double datodb);
    void setDbByte(int numdb,int areastart,int datodb);
    void setBit(QString area,int numdb ,int byte_adr ,int bit_adr);
    void resetBit(QString area,int numdb ,int byte_adr ,int bit_adr);



protected:
    daveConnection *dc;
    int ok_con;
    int ok_plc,ok_tcp;
    int buf_byte;
    int error;

private:
    int handle;
    int dbnum;
    int areastart;
    int arealen;
    int type_area;
    float datodb;
    int buf_int;
    int buf_real;
    int buf_word;
    double buf_dword;
    QByteArray db;
    QTcpSocket *tcp;
    _daveOSserialType fds;
    daveInterface *di;

signals:

/**
 * Signal emit for plc connection ok
*/
    void s_plcCon(int);

public slots:


};

#endif // PLCQTLIB_H
