#ifndef DISTANCEBEARING_H
#define DISTANCEBEARING_H

#include <QDialog>

namespace Ui {
class DistanceBearing;
}

class DistanceBearing : public QDialog
{
    Q_OBJECT

public:
    DistanceBearing(QWidget *parent = nullptr);
    ~DistanceBearing();

private slots:
    void on_buttonBox_accepted();

public:
    QString d_pointA;
    QString d_pointB;

private:
    Ui::DistanceBearing *ui;
};

#endif // DISTANCEBEARING_H
