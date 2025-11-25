#include "transcriptreviewer.h"
#include "ui_transcriptreviewer.h"
#include "autotab.h"

TranscriptReviewer::TranscriptReviewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TranscriptReviewer)
{
    ui->setupUi(this);

    // Setup auto-tab
    setupAutoTab(ui->lineEdit_StartRow, 2);
    setupAutoTab(ui->lineEdit_EndRow, 2);
    setupAutoTab(ui->lineEdit_FICE, 6);
}

TranscriptReviewer::~TranscriptReviewer()
{
    delete ui;
}

void TranscriptReviewer::on_pushButtonHome_clicked()
{
    this->hide();
    if(parent() != nullptr) {
        QWidget *parentWidget = qobject_cast<QWidget*>(parent());
        if (parentWidget != nullptr) {
            parentWidget->show();
        }
    }
}

