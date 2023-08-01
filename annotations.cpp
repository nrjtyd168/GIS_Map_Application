#include "annotations.h"
#include "ui_annotations.h"

Annotations::Annotations(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Annotations)
{
    ui->setupUi(this);

    // Create Annotations Table Model
    antTableModel = new QSqlTableModel();
    antTableModel->setTable("annotations");
    antTableModel->select();
    antTableModel->sort(0, Qt::AscendingOrder);

    ui->antTableView->setModel(antTableModel);

}

Annotations::~Annotations()
{
    delete ui;
}
