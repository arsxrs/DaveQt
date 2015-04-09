//  project by Vincenzo D'Agostino (vindagos@gmail.com)


#include "plcqtlib.h"


Plcqtlib::Plcqtlib()
{
    dbnum = 0;
    areastart = 0;
    arealen = 0;
    error = -1;
    ok_plc = 1;
    ok_tcp = -1;
    type_area = 0;
    ok_con = 1;
    tcp = new  QTcpSocket();


}

Plcqtlib::~Plcqtlib()
{


}



/**
 * @brief Plcqtlib::intToBitArray
 *\n
 * Converts an int into an array of bits
 *
 * @param num Integer to convert
 * @param size array size
 * @return value in QBitarray
 */

QBitArray Plcqtlib:: intToBitArray(int num , int size)
{
    QBitArray  qba;
    int div = 2;
    int resto = 0;
    int i= 0;

    do{
        resto = num % div;
        if(resto != 0){
            qba.setBit(i,true);
            }else if(resto == 0){
            qba.setBit(i,false);
            }
        num = num/2;
        i++;
        }while(i<=size);

    return qba;
}

/**
 * @brief Plcqtlib::BitArrayToInt
 *\n
 * Converts an array of bits in an int (max 16 bit)
 *
 * @param toint QBitArray
 * @param nbit number of bits to be converted (see notes)
 * @return int 16-bits
 */

int Plcqtlib::BitArrayToInt(QBitArray toint ,int nbit)
{
    int num = 0;
    int temp = 0;
    int stato = 0;
    bool test = false;
    int i = -1;

    for (i=0 ; i<nbit ; i++){
        test = toint.testBit(i);
        if(test == false){
            stato = 0;
        }else if (test == true){
            stato = 1;
        }
        temp = stato *pow(2,i);
        num = num + temp;
     }

    return num;
}

/**
 * @brief Plcqtlib::setTcp
 *\n
 * Setting ip addres and establishes the connection to the network
 *
 * @param port (default 102)
 * @param ip IP address of the partner
 * @return  -1 = error , 0 = ok
 */

int Plcqtlib::setTcp(QString ip , int port)
{
    tcp->connectToHost(ip,port); // connessione
    if (tcp->waitForConnected(3000)){
        ok_tcp = 0;
        handle = tcp->socketDescriptor(); // ottengo il socket descriptor
    }else  if (tcp->state() == QTcpSocket::UnconnectedState || tcp->waitForDisconnected(3000)){
        ok_tcp = -1;
        tcp->disconnectFromHost(); // disconnesione
    }
    return ok_tcp;

}



/**
 * @brief Plcqtlib::setPlc
 *\n
 * Establishes the connection to the plc
 *\n
 * @param plctype = s7300 , s7200 , logo , S71200
 * no case sensitive
 * @param mpi 2(S7300), 1(Logo oba7),2(S7200) , 2(S71200)
 * @param rack 0(S7300), 1(Logo oba7), 0(S7200),1(S71200)
 * @param slot 2(S7300), 0(Logo oba7),0(S7200),0(S71200)
 * @param plctype = s7300 , s7200 , logo-0BA7 , S71200
 * @return 0 = connection ok , 1 = error
 */
int Plcqtlib::setPlc(QString plctype ,  int mpi , int rack , int slot)
{
    int plc = 0;
    char *if1;
    char nname [4]={'I','F','1'}; //correzzione per versione deprecata
    if1 = nname; //conversione array to char pointer


    if (plctype == "s7300" || plctype == "S7300" ){
        plc = 122;
        ok_plc = 0;
    }else if(plctype == "s7200" || plctype == "S7200"){
        plc = 123;
        ok_plc = 0;
    }else if(plctype == "logo" || plctype == "LOGO"){
        plc = 122;
        ok_plc = 0;
    }else if(plctype == "s71200" || plctype == "S71200"){
        plc = 122;
        ok_plc = 0;
    }else if((plc != 122) & (plc != 123)){
    ok_plc = 1; //errore
    }

    if(ok_plc == 0){
        fds.wfd =(HANDLE)handle;
        fds.rfd = fds.wfd;
        if (fds.rfd>0) {
            di = daveNewInterface(fds, if1,0, plc, daveSpeed187k);
            daveSetTimeout(di,5000000);
            dc=daveNewConnection(di,mpi,rack,slot);
            // collegamento al plc
            if (daveConnectPLC(dc)== 0){
                ok_plc=0;
            }else ok_plc=1;
        }
    }
    if((ok_plc == 0)&(ok_tcp == 0)){
        ok_con = 0;
        emit this->s_plcCon(ok_con);

    }
    return ok_con;

}


/**
 * @brief Plcqtlib::getSocketDescriptor
 * @return returns the socket descriptor
 */

int Plcqtlib::getSocketDescriptor() const
{
    return tcp->socketDescriptor();
}


/**
 * @brief Plcqtlib::getConnectionPlc
 * @return returns the connection plc
 */

daveConnection *Plcqtlib::getConnectionPlc() const
{
    return dc;
}

/**
 * @brief Plcqtlib::getHaddress
 * @return returns the address of the partner
 */

QString Plcqtlib::getAddress() const
{
    QHostAddress address;
    address=tcp->peerAddress();
    return address.toString();
}


/**
 * @brief Plcqtlib::getPort
 * @return returns the port of the partner
 */

int Plcqtlib::getPort() const
{

    return tcp->peerPort();

}

/**
 * @brief Plcqtlib::getError
 *\n
 * Detects the general error message
 *\n
 * Valid for : S7300 - S7400 - S7200 - LOGO 0BA7 - S71200
 *
 * @return returns the error message
 * @brief mess = "ok" no error
 */
QString Plcqtlib::getError() const
{
    char *er;
    er = daveStrerror(error);
    QString mess(er);
    return mess;
}


/**
 * @brief Plcqtlib::disconnectPlc
 *\n
 * Disconnects from the PLC and the network
 *\n
 * Valid for : S7300 - S7400
 *
 * @return returns the connection status
 */

int Plcqtlib::disconnectPlc()const
{
    int stato = -1;
    tcp->disconnectFromHost();
    stato = daveDisconnectPLC(dc);
    daveFree(dc);
    return stato;
}

/**
 * @brief Plcqtlib::stopPlc
 *\n
 * Valid for : S7300 - S7400
 *
 * @return returns 1 if the stop of a successful plc
 */

int Plcqtlib::stopPlc() const
{
    int retstop = 1;
    retstop=daveStop(dc);
    return retstop;
}

/**
 * @brief Plcqtlib::runPlc
 *\n
 * Valid for : S7300 - S7400
 *
 * @return returns 1 if the plc and run
 */

int Plcqtlib::runPlc() const
{
    int retrun = 1;
    retrun=daveStart(dc);
    return retrun;
}

/**
 * @brief Plcqtlib::getDaveBlockList
 *\n
 * Gets the data blocks DB in the plc
 *\n
 * Valid for : S7300 - S7400
 *
 * @return returns the data blocks in the PLC in a QList <QString>
 */

QList <QString> Plcqtlib::getDaveBlockList()
{
    QString str[256],str1;
    QList<QString> list;

    int l, i; // l = ritorna il numero di blocchi
    daveBlockEntry dbe[256]; //per il numero del blocco
    daveBlockInfo db[256]; // per i numeri dei blocchi
    l=daveListBlocksOfType(dc,daveBlockType_DB, dbe);

    if (l<0) {
        error = l;
        }else
        for (i=0; i<l; i++) {
            daveGetBlockInfo(dc, db, daveBlockType_DB,dbe[i].number);
            str[i] = daveBlockName(daveBlockType_DB); //tipo di blocco "DB" in stringa
            str[i].append(str1.setNum(dbe[i].number)); // numero del blocco DB trasf. in stringa
            list.append(str[i]);
            }
     return list;

}


/**
 * @brief Plcqtlib::getDbInt
 *\n
 * Reads a 16-bit integer from a data block (-32768 a +32767)
 *\n
 * Valid for : S7300 - S7400 - S7200 - LOGO 0BA7 - S71200
 *\n
 * S7200 numdb = 0 (Area V)
 *\n
 * LOGO 0BA7  numdb = 0 (Area V)
 *
 * @param numdb number of the data block
 * @param areastart start address of the data
 * @return returns the value read
 */

int Plcqtlib::getDbInt(int numdb,int areastart)
{
    int dbint = 0;
    error = daveReadBytes(dc,daveDB,numdb,areastart,WORD,NULL);
    if (error == 0){
        dbint=daveGetU16(dc);
    }
    return dbint;
}


/**
 * @brief Plcqtlib::getDbDint
 *\n
 * Reads a 32-bit integer from a data block (-2147483648 a +2147483647)
 *\n
 * Valid for : S7300 - S7400 - S7200 - S71200
 *\n
 * S7200 numdb = 0 (Area V)
 *
 * @param numdb  number of the data block
 * @param areastart start address of the data
 * @return returns the value read
 */

double Plcqtlib::getDbDint(int numdb,int areastart)
{

    int dbdint = 0;
    error = daveReadBytes(dc,daveDB,numdb,areastart,DWORD,NULL);
    if (error == 0){
        dbdint=daveGetS32(dc);
    }
    return dbdint;

}

/**
 * @brief Plcqtlib::getDbReal
 *\n
 * Reads a floating point number (1.234567e+13)
 *\n
 * Valid for : S7300 - S7400 - S7200 - S71200
 *\n
 *S7200 numdb = 0 (Area V)
 *
 * @param numdb number of the data block
 * @param areastart start address of the data
 * @return returns the value read
 */

float Plcqtlib::getDbReal(int numdb,int areastart)
{
    float dbreal = 0;
    error = daveReadBytes(dc,daveDB,numdb,areastart,DWORD,NULL);
    if (error == 0){
        dbreal=daveGetFloat(dc);

    }
    return dbreal;

}


/**
 * @brief Plcqtlib::getDbWord
 * \n
 * Reads a 16-bit word from a data block (-999 a +999)
 *\n
 * Valid for : S7300 - S7400 - S7200 - LOGO 0BA7 - S71200
 *\n
 * S7200 numdb = 0 , Logo 0BA7 numdb = 0 (Area V)
 *
 * @param numdb number of the data block
 * @param areastart start address of the data
 * @return returns the value read
 */

int Plcqtlib::getDbWord(int numdb,int areastart)
{

    int dbword = 0;
    error = daveReadBytes(dc,daveDB,numdb,areastart,WORD,NULL);
    if (error == 0){
        dbword=daveGetU16(dc);
    }
    return dbword;

}


/**
 * @brief Plcqtlib::getDbDword
 *\n
 * Reads a 32-bit double word from a data block (-9999999 a +9999999)
 *\n
 * Valid for : S7300 - S7400 - S7200 - S71200
 *\n
 * S7200 numdb = 0 (Area V)
 *
 * @param numdb number of the data block
 * @param areastart start address of the data
 * @return returns the value read
 */

double Plcqtlib::getDbDword(int numdb,int areastart)
{
    int dbdword = 0;
    error = daveReadBytes(dc,daveDB,numdb,areastart,DWORD,NULL);
    if (error == 0){
        dbdword=daveGetU32(dc);
    }
    return dbdword;
}


/**
 * @brief Plcqtlib::getDbByte
 *\n
 * Reads a byte from a data block
 *\n
 * Valid for : S7300 - S7400 - S7200 - LOGO 0BA7 - S71200
 *\n
 * S7200 e Logo 0BA7 numdb = 0 (Area V)
 *
 * @param numdb number of the data block
 * @param areastart start address of the data
 * @return returns the value read
 */

int Plcqtlib::getDbByte(int numdb,int areastart)
{
    int dbbyte = 0;
    error = daveReadBytes(dc,daveDB ,numdb,areastart,BYTE,NULL);
    if (error == 0){
        dbbyte=daveGetU8(dc);
    }
    return dbbyte;

}


/**
 * @brief Plcqtlib::getBitArray
 *\n
 * Reads the individual bits from one area data (max 16 bit)
 *\n
 * Valid for : S7300 - S7400 - S7200 - LOGO 0BA7 - S71200
 *
 * @param numdb  number of the data block
 *\n
 * S7200 e Logo 0BA7 numdb = 0 (Area V)
 *
 * @param area area data
 *\n
 * QString value -> "DB" = data block ,"MEM" = area merker ,"OUT" = output ,"IN" = input
 *
 * @param areastart start address of the data
 * @param arealen number of bits
 * @return returns the value read in an array of bits
 */

QBitArray Plcqtlib::getBitArray(QString area ,int numdb, int areastart, int arealen)
{
    int bytea,byteb,bit16;
    int val = 0;
    int dbbit = 0;
    QBitArray  qba;
    if (area == "db" || area == "DB"){
            val=daveDB;

        }else  if (area == "in" || area == "IN"){
            val=daveInputs;
            numdb = 0;

        }else  if (area == "out" || area == "OUT"){
            val=daveOutputs;
            numdb = 0;
        }else  if (area == "mem" || area == "MEM"){
            val=daveFlags;
            numdb = 0;
        }
    type_area = val;
    error = daveReadBytes(dc,type_area,numdb,areastart,WORD,NULL);
    if (error == 0){
        bit16 = daveGetU16(dc);
        bytea = bit16 >> 8;
        byteb = bit16 << 8;
        dbbit = byteb|bytea;
        }
    qba = intToBitArray(dbbit,arealen);
    return qba;
}


// SCRITTURA DATI SU PLC

/**
 * @brief Plcqtlib::setDbInt
 *\n
 * Writes a 16-bit integer on a data block(-32768 a +32767)
 *\n
 * Valid for : S7300 - S7400 - S7200 - S71200
 *\n
 * S7200 e Logo 0BA7 numdb = 0 (Area V)
 *
 * @param numdb number of the data block
 * @param areastart start address of the data
 */

void Plcqtlib::setDbInt(int numdb,int areastart ,int datodb)
{

    buf_int = daveSwapIed_16((int)datodb);
    error = daveWriteBytes(dc,daveDB,numdb,areastart,WORD,&buf_int);

}

/**
 * @brief Plcqtlib::setDbDint
 *\n
 * Writes a 32-bit integer on a data block (-2147483648 a +2147483647)
 *\n
 * Valid for : S7300 - S7400 - S7200 - S71200
 *\n
 * S7200 numdb = 0 (Area V)
 *
 * @param numdb number of the data block
 * @param areastart start address of the data
 */

void Plcqtlib::setDbDint(int numdb,int areastart ,double datodb)
{

    buf_int = daveSwapIed_32((int)datodb);
    error = daveWriteBytes(dc,daveDB,numdb,areastart,DWORD,&buf_int);

}



/**
 * @brief Plcqtlib::setDbReal
 *\n
 * Writes a floating point number of a data block (1.234567e+13)
 *\n
 * Valid for : S7300 - S7400 - S7200 - S71200
 *\n
 * S7200 numdb = 0 (Area V)
 *
 * @param numdb number of the data block
 * @param areastart start address of the data
 */

void Plcqtlib::setDbReal(int numdb,int areastart, float datodb)
{
    buf_real =  daveToPLCfloat(datodb);
    error = daveWriteBytes(dc,daveDB,numdb,areastart,DWORD,&buf_real);

}


/**
 * @brief Plcqtlib::setDbWord
 *\n
 * Writes a 16-bit word of a data block (-999 a +999)
 *\n
 * Valid for : S7300 - S7400 - S7200 - LOGO 0BA7 - S71200
 *\n
 * S7200 e Logo 0BA7 numdb = 0 (Area V)
 *
 * @param numdb number of the data block
 * @param areastart start address of the data
 */

void Plcqtlib::setDbWord(int numdb,int areastart,int datodb)
{
    buf_word = daveSwapIed_16((short)datodb);
    error = daveWriteBytes(dc,daveDB,numdb,areastart,WORD,&buf_word);

}



/**
 * @brief Plcqtlib::setDbDword
 *\n
 * Writes a 32-bit word of a data block (-9999999 a +9999999)
 *\n
 * Valid for : S7300 - S7400 - S7200 - S71200
 *\n
 * S7200 numdb = 0 (Area V)
 *
 * @param numdb number of the data block
 * @param areastart start address of the data
 */

void Plcqtlib::setDbDword(int numdb,int areastart,double datodb)
{
    buf_word = daveSwapIed_32((int)datodb);
    error = daveWriteBytes(dc,daveDB,numdb,areastart,DWORD,&buf_word);

}



/**
 * @brief Plcqtlib::setDbByte
 *\n
 * Writes a byte of a data block
 *\n
 * Valid for : S7300 - S7400 - S7200 - LOGO 0BA7 - S71200
 *\n
 * S7200 e Logo 0BA7 numdb = 0 (Area V)
 *
 * @param numdb number of the data block
 * @param areastart start address of the data
 */

void Plcqtlib::setDbByte(int numdb,int areastart,int datodb)
{
    buf_byte = (int)datodb;
    error =  daveWriteBytes(dc,daveDB,numdb,areastart,BYTE,&buf_byte);

}



/**
 * @brief Plcqtlib::setBit
 *\n
 * Writes the individual bits of an area of a data block
 *\n
 * Valid for : S7300 - S7400 - S7200 - LOGO 0BA7 - S71200
 *
 * @param numdb  :number of the data block
 *\n
 * S7200 e Logo 0BA7 numdb = 0 (Area V)
 *
 * @param area area data
 *\n
 * QString value -> "DB" = data block ,"MEM" = area merker ,"OUT" = output
 *
 * @param byte_adr byte address
 * @param  bit_adr address of the bit to be set
 */

void Plcqtlib::setBit(QString area,int numdb ,int byte_adr ,int bit_adr)
{
    int val = 0;
    if (area == "db" || area == "DB"){
        val=daveDB;

    }else  if (area == "mem" || area == "MEM"){
        val=daveFlags;
        numdb = 0;
    }else  if (area == "out" || area == "OUT"){
        val=daveOutputs;
        numdb = 0;
    }

    type_area = val;

    error = daveSetBit(dc,type_area ,numdb,byte_adr,bit_adr);

}



/**
 * @brief Plcqtlib::resetBit
 *\n
 * Reset the individual bits from one area of a data block
 *\n
 * Valid for : S7300 - S7400 - S7200 - LOGO 0BA7 - S71200
 *
 * @param numdb number of the data block
 *\n
 * S7200 e Logo 0BA7 numdb = 0 (Area V)
 *
 * @param area area data
 *\n
 * QString value -> "DB" = data block ,"MEM" = area merker ,"OUT" = output
 *
 * @param byte_adr byte address
 * @param  bit_adr address of the bit to be reset
 */

void Plcqtlib::resetBit(QString area, int numdb , int byte_adr , int bit_adr)
{
    int val = 0;
    if (area == "db" || area == "DB"){
        val=daveDB;

    }else  if (area == "mem" || area == "MEM"){
        val=daveFlags;
        numdb = 0;
    }else  if (area == "out" || area == "OUT"){
        val=daveOutputs;
        numdb = 0;
    }

    type_area = val;
    error = daveClrBit(dc,type_area ,numdb,byte_adr,bit_adr);

}
