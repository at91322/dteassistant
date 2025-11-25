#include "articulator.h"
#include "ui_articulator.h"

Articulator::Articulator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Articulator)
{
    ui->setupUi(this);
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

