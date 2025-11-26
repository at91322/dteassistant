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

    void on_actionChange_Username_triggered();
    void on_actionChange_Current_Term_triggered();
    void on_actionAlways_On_Top_triggered(bool checked);

private:
    Ui::DiplomaDates *ui;

    StatusBarManager *statusBarManager;
};

#endif // DIPLOMADATES_H
