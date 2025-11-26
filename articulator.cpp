#include "articulator.h"
#include "ui_articulator.h"
#include "autotab.h"
#include "enterkeyhandler.h"

#include <QDir>
#include <QFrame>
#include <QSettings>

Articulator::Articulator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Articulator)
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

Articulator::~Articulator()
{
    delete ui;
}

void Articulator::on_pushButtonHome_clicked()
{
    this->hide();
    if(parent() != nullptr) {
        QWidget *parentWidget = qobject_cast<QWidget*>(parent());
        if (parentWidget != nullptr) {
            parentWidget->show();
        }
    }
}

