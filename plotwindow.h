#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QWidget>
#include "qcustomplot.h"
namespace Ui {
class PlotWindow;
}

class PlotWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PlotWindow(QWidget *parent = 0);
    ~PlotWindow();

     QCustomPlot *customPlot;
     QDateTimeEdit *UpdateTimeValue;
     QDateTimeEdit *VisiblePeriodValue;

private:
    Ui::PlotWindow *ui;
};

#endif // PLOTWINDOW_H
