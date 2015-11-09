#include "item.h"

Item::Item(QTableWidget *Table, int rowCount )
{
    table = Table;
    row = rowCount;
    table->insertRow(rowCount);
}

Item::~Item()
{

}

void Item::Init()
{
    checkBoxEn.setStyleSheet("margin-left:18%;");
    checkBoxGraph.setStyleSheet("margin-left:15%;");
    table->setItem(row, 0, &Address);
    table->setItem(row, 1, &Type);
    table->setItem(row, 3, &Value);

    table->setCellWidget(row, 2, &checkBoxEn);
    table->setCellWidget(row, 4, &checkBoxGraph);
    Type.setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

}




void Item::setType(QString  str){

    Type.setText(str);


}
void Item::setValue(QString  str){

    Value.setText(str);


}

void Item::ParseAddressStr(){


    //first search digit
    int srchNum =1;
    int listLen = 0;

    QString inputStr = Address.text();

    int len = inputStr.length();

    if (len){

        inputStr = inputStr.toUpper();
        inputStr = inputStr.trimmed();
        inputStr = inputStr.replace(","," ");
        inputStr = inputStr.replace("."," ");

        len = inputStr.length();




        /* search digit and symbol
         * format "DB1.DBx1"
         *        "DB1.DBx1.0"
         */
        int i = 0;
        while (i <= len){
            //Search digit
            if( srchNum == 1 && (inputStr[i].isDigit())){
                inputStr.insert(i, " ");
                i++;
                len+=1 ;
                srchNum = 0;
            }
            //Search symbol
            if( srchNum == 0 && (!inputStr[i].isDigit())){
                inputStr.insert(i, " ");
                srchNum = 1;
            }

            i++;

        }

        //Remove double spaces
        while(inputStr.contains("  ")){
            inputStr.replace("  "," ");
        }

        //Separate string to list and get it length
        ListParam = inputStr.split(" ");
        listLen=ListParam.length();


        if (listLen<2){
            Address.setBackgroundColor(Qt::red);
            return;
        }
        else{
            Address.setBackgroundColor(Qt::green);
            Address.setText(inputStr);

            for (int i=0; i<listLen;i++)
                qDebug()<<i+1<<" = "<<ListParam[i];

        }

    }
    else{ //string empty

        Address.setBackgroundColor(Qt::white);
    }

}

bool Item::getEn(){
    return checkBoxEn.checkState();
}
