#include "distancebearing.h"
#include "ui_distancebearing.h"

DistanceBearing::DistanceBearing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DistanceBearing)
{
    ui->setupUi(this);
}

DistanceBearing::~DistanceBearing()
{
    delete ui;
}

void DistanceBearing::on_buttonBox_accepted()
{
     d_pointA = ui->d_lePointA->text();
     d_pointB = ui->d_lePointB->text();
}

