#include "diplomadates.h"
#include "ui_diplomadates.h"
#include "autotab.h"

DiplomaDates::DiplomaDates(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DiplomaDates)
{
    ui->setupUi(this);

    // Setup auto-tab
    setupAutoTab(ui->lineEdit_StartRow, 2);
    setupAutoTab(ui->lineEdit_EndRow, 2);
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

