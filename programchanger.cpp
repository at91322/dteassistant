#include "programchanger.h"
#include "ui_programchanger.h"
#include "enterkeyhandler.h"

ProgramChanger::ProgramChanger(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProgramChanger)
{
    ui->setupUi(this);

    // Setup status bar
    statusBarManager = new StatusBarManager(ui->statusbar, this);
    statusBarManager->startMonitoring();

    // Setup enter/return handling for all buttons
    setupEnterKeyForButtons(this);

    // Make the Always On Top action checkable
    ui->actionAlways_On_Top->setCheckable(true);
}

ProgramChanger::~ProgramChanger()
{
    delete ui;
}

void ProgramChanger::on_pushButtonHome_clicked()
{
    this->hide();
    if(parent() != nullptr) {
        QWidget *parentWidget = qobject_cast<QWidget*>(parent());
        if (parentWidget != nullptr) {
            parentWidget->show();
        }
    }
}

void ProgramChanger::on_actionChange_Username_triggered()
{
    statusBarManager->updateUsername(this);
}

void ProgramChanger::on_actionChange_Current_Term_triggered()
{
    statusBarManager->updateCurrentTerm(this);
}

void ProgramChanger::on_actionAlways_On_Top_triggered(bool checked)
{
    statusBarManager->toggleAlwaysOnTop(this, checked);
}
