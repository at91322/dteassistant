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

