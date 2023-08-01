#include "waypoints.h"
#include "ui_waypoints.h"

Waypoints::Waypoints(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Waypoints)
{
    ui->setupUi(this);

    // Create Annotations Table Model
    wptTableModel = new QSqlTableModel();
    wptTableModel->setTable("waypoints");
    wptTableModel->select();
    wptTableModel->sort(0, Qt::AscendingOrder);

    ui->wptTableView->setModel(wptTableModel);
}

Waypoints::~Waypoints()
{
    delete ui;
}
