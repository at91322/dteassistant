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

