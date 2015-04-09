#include "item.h"

Item::Item(QTableWidget *Table, int rowCount )
{

    table = Table;
    row = rowCount;
    table->insertRow(rowCount);

    checkBoxEn.setStyleSheet("margin-left:18%;");
    checkBoxGraph.setStyleSheet("margin-left:15%;");

    table->setItem(row, 0, &Address);
    table->setItem(row, 1, &Type);
    table->setCellWidget(row, 2, &checkBoxEn);
    table->setItem(row, 3, &Value);
    table->setCellWidget(row, 4, &checkBoxGraph);


    Type.setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

}

Item::~Item()
{

}

void Item::setType(QString  str){

    Type.setText(str);


}
void Item::setValue(QString  str){

    Value.setText(str);


}

void Item::ParseAddressStr(){
    int len=0;
    int i =0;
    int srchNum =1;

    int listLen = 0;

    QString iStr = Address.text();



    len = iStr.length();
    if (len)    {

        iStr = iStr.toUpper();
        iStr = iStr.trimmed();
        iStr = iStr.replace("."," ");

        len = iStr.length();



        while (i < len){

            if( srchNum == 1 && (iStr[i].isDigit()) ){
                iStr.insert(i, " ");
                // i++;
                len+=1 ;
                srchNum = 0;
            }

            if( srchNum == 0 && (!iStr[i].isDigit()) ){
                iStr.insert(i, " ");
                srchNum = 1;
            }

            i++;

        }


        while(iStr.contains("  ")){
            iStr.replace("  "," ");
        }

        ListParam = iStr.split(" ");
        listLen=ListParam.length();


        if (listLen<2){
            Address.setBackgroundColor(Qt::red);
            return;
        }





        Address.setBackgroundColor(Qt::red);







        //    qDebug()<<"dbnum "<<ListParam[1];
        //    qDebug()<<"start "<<ListParam[3];
        //    qDebug()<<"bitno "<<ListParam[4];
        //Str = Replace(Str, ".", " ")


    }
    else{
        Address.setBackgroundColor(Qt::white);
    }

}


