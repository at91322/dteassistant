#ifndef TERMBUILDER_H
#define TERMBUILDER_H

#include "statusbarmanager.h"

#include <QMainWindow>

namespace Ui {
class TermBuilder;
}

class TermBuilder : public QMainWindow
{
    Q_OBJECT

public:
    explicit TermBuilder(QWidget *parent = nullptr);
    ~TermBuilder();

private slots:
    void on_pushButtonHome_clicked();

    void on_actionChange_Username_triggered();
    void on_actionChange_Current_Term_triggered();
    void on_actionChange_Default_Web_Browser_triggered();
    void on_actionAlways_On_Top_triggered(bool checked);

private:
    Ui::TermBuilder *ui;

    StatusBarManager *statusBarManager;
};

#endif // TERMBUILDER_H
