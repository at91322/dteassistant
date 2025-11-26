#ifndef DIPLOMADATES_H
#define DIPLOMADATES_H

#include "statusbarmanager.h"

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

    StatusBarManager *statusBarManager;
};

#endif // DIPLOMADATES_H
