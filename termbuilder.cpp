#include "termbuilder.h"
#include "ui_termbuilder.h"
#include "autotab.h"

TermBuilder::TermBuilder(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TermBuilder)
{
    ui->setupUi(this);

    // Setup auto-tab
    setupAutoTab(ui->lineEdit_StartRow, 2);
    setupAutoTab(ui->lineEdit_EndRow, 2);
    setupAutoTab(ui->lineEdit_FICE, 6);
    setupAutoTab(ui->lineEdit_InstNum, 2);
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

