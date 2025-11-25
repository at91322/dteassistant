#ifndef DIPLOMADATES_H
#define DIPLOMADATES_H

#include <QMainWindow>

namespace Ui {
class DiplomaDates;
}

class DiplomaDates : public QMainWindow
{
    Q_OBJECT

public:
    explicit DiplomaDates(QWidget *parent = nullptr);
    ~DiplomaDates();

private slots:
    void on_pushButtonHome_clicked();

private:
    Ui::DiplomaDates *ui;
};

#endif // DIPLOMADATES_H
