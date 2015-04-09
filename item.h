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

class Item : public QObject
{
    Q_OBJECT
public:
    explicit Item(QTableWidget *Table, int rowCount );
    ~Item();

    void setValue(QString  str);
    void setType(QString  str);
    void ParseAddressStr();

    QTableWidget *table;
    QTableWidgetItem *tableItem;

    QStringList ListParam;


    QCheckBox checkBoxEn;
    QCheckBox checkBoxGraph;
    QTableWidgetItem Address;
    QTableWidgetItem Type;
    QTableWidgetItem Value;


    int dbnum;
    int areastart;
    int arealen;
    int type_area;
    int a;
private:
    int row;

public slots:
    //    void StrParse();
    //    void SelectType();

signals:

public slots:
    //    void StrParse();
    //    void SelectType();/////
};

#endif // ITEM_H












