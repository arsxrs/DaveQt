#ifndef ITEM_H
#define ITEM_H

#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QString>
#include <QStringList>
#include <QHBoxLayout>
#include <QDebug>

#include "qcustomplot.h"

class Item : public QObject
{
    Q_OBJECT
public:
    explicit Item(QTableWidget *Table, int rowCount);
    ~Item();
    void Init();
    void ParseAddressStr();
    void setType(QString  str);
    void setValue(QString  str);
    bool getEn();
        QTableWidgetItem Value;
 QCPGraph *graph;
private:

    QTableWidget *table;
    QTableWidgetItem *tableItem;
    QCheckBox checkBoxEn;
    QCheckBox checkBoxGraph;
    QTableWidgetItem Address;
    QTableWidgetItem Type;

    QStringList ListParam;
    int row;
    int dbnum;
    int areastart;
    int arealen;
    int type_area;
    int a;



public slots:
    //    void StrParse();
    //    void SelectType();

signals:

public slots:
    //    void StrParse();
    //    void SelectType();/////
};

#endif // ITEM_H












