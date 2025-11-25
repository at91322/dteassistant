#include "diplomadates.h"
#include "ui_diplomadates.h"

DiplomaDates::DiplomaDates(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DiplomaDates)
{
    ui->setupUi(this);
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

