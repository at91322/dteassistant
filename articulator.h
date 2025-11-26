#ifndef ARTICULATOR_H
#define ARTICULATOR_H

#include "statusbarmanager.h"

#include <QLabel>
#include <QMainWindow>

namespace Ui {
class Articulator;
}

class Articulator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Articulator(QWidget *parent = nullptr);
    ~Articulator();

private slots:
    void on_pushButtonHome_clicked();

    void on_actionChange_Username_triggered();
    void on_actionChange_Current_Term_triggered();

private:
    Ui::Articulator *ui;

    StatusBarManager *statusBarManager;
};

#endif // ARTICULATOR_H
