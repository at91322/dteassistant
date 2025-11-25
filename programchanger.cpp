#include "programchanger.h"
#include "ui_programchanger.h"

ProgramChanger::ProgramChanger(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProgramChanger)
{
    ui->setupUi(this);
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

