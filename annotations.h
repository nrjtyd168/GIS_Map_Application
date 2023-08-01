#ifndef ANNOTATIONS_H
#define ANNOTATIONS_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class Annotations;
}

class Annotations : public QDialog
{
    Q_OBJECT

public:
    explicit Annotations(QWidget *parent = nullptr);
    ~Annotations();

private:
    Ui::Annotations *ui;

public:
    QSqlTableModel* antTableModel;
};

#endif // ANNOTATIONS_H
