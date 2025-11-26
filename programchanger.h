#ifndef PROGRAMCHANGER_H
#define PROGRAMCHANGER_H

#include "statusbarmanager.h"

#include <QMainWindow>

namespace Ui {
class ProgramChanger;
}

class ProgramChanger : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProgramChanger(QWidget *parent = nullptr);
    ~ProgramChanger();

private slots:
    void on_pushButtonHome_clicked();

private:
    Ui::ProgramChanger *ui;

    StatusBarManager *statusBarManager;
};

#endif // PROGRAMCHANGER_H
