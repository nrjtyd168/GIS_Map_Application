#ifndef WAYPOINTS_H
#define WAYPOINTS_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class Waypoints;
}

class Waypoints : public QDialog
{
    Q_OBJECT

public:
    explicit Waypoints(QWidget *parent = nullptr);
    ~Waypoints();

private:
    Ui::Waypoints *ui;

public:
    QSqlTableModel* wptTableModel;
};

#endif // WAYPOINTS_H
