#include "transcriptreviewer.h"
#include "ui_transcriptreviewer.h"

TranscriptReviewer::TranscriptReviewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TranscriptReviewer)
{
    ui->setupUi(this);
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

