#include "plotwindow.h"
#include "ui_plotwindow.h"

PlotWindow::PlotWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotWindow)
{
    ui->setupUi(this);\
    customPlot = ui->custPlot;
    UpdateTimeValue = ui->UpdateTime;
    VisiblePeriodValue = ui->VisivblePeriod;
}

PlotWindow::~PlotWindow()
{
    delete ui;
}
