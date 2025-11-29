#include "diplomadates.h"
#include "ui_diplomadates.h"

#include "autotab.h"
#include "enterkeyhandler.h"

DiplomaDates::DiplomaDates(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DiplomaDates)
{
    ui->setupUi(this);

    // Setup status bar
    statusBarManager = new StatusBarManager(ui->statusbar, this);
    statusBarManager->startMonitoring();

    // Setup auto-tab
    setupAutoTab(ui->lineEdit_StartRow, 2);
    setupAutoTab(ui->lineEdit_EndRow, 2);

    // Setup enter/return handling for all buttons
    setupEnterKeyForButtons(this);

    // Make the Always On Top action checkable
    ui->actionAlways_On_Top->setCheckable(true);
}

DiplomaDates::~DiplomaDates()
{
    delete ui;
}

void DiplomaDates::on_pushButtonHome_clicked()
{
    this->hide();
    if(parent() != nullptr) {
        QWidget *parentWidget = qobject_cast<QWidget*>(parent());
        if (parentWidget != nullptr) {
            parentWidget->show();
        }
    }
}

void DiplomaDates::on_actionChange_Username_triggered()
{
    statusBarManager->updateUsername(this);
}

void DiplomaDates::on_actionChange_Current_Term_triggered()
{
    statusBarManager->updateCurrentTerm(this);
}

void DiplomaDates::on_actionChange_Default_Web_Browser_triggered()
{
    statusBarManager->updateBrowser(this);
}

void DiplomaDates::on_actionAlways_On_Top_triggered(bool checked)
{
    statusBarManager->toggleAlwaysOnTop(this, checked);
}

