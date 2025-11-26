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

private:
    Ui::TermBuilder *ui;

    StatusBarManager *statusBarManager;
};

#endif // TERMBUILDER_H
