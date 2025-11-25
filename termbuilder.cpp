#include "termbuilder.h"
#include "ui_termbuilder.h"

TermBuilder::TermBuilder(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TermBuilder)
{
    ui->setupUi(this);
}

TermBuilder::~TermBuilder()
{
    delete ui;
}

void TermBuilder::on_pushButtonHome_clicked()
{
    this->hide();
    if(parent() != nullptr) {
        QWidget *parentWidget = qobject_cast<QWidget*>(parent());
        if (parentWidget != nullptr) {
            parentWidget->show();
        }
    }
}

