#include "transcriptreviewer.h"
#include "ui_transcriptreviewer.h"

#include "configmanager.h"
#include "autotab.h"
#include "enterkeyhandler.h"

TranscriptReviewer::TranscriptReviewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TranscriptReviewer)
{
    ui->setupUi(this);

    // Setup status bar
    statusBarManager = new StatusBarManager(ui->statusbar, this);
    statusBarManager->startMonitoring();

    // Setup auto-tab
    setupAutoTab(ui->lineEdit_StartRow, 2);
    setupAutoTab(ui->lineEdit_EndRow, 2);
    setupAutoTab(ui->lineEdit_FICE, 6);

    // Setup enter/return handling for all buttons
    setupEnterKeyForButtons(this);

    // Make the Always On Top action checkable
    ui->actionAlways_On_Top->setCheckable(true);
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

void TranscriptReviewer::on_actionChange_Username_triggered()
{
    statusBarManager->updateUsername(this);
}

void TranscriptReviewer::on_actionChange_Current_Term_triggered()
{
    statusBarManager->updateCurrentTerm(this);
}

void TranscriptReviewer::on_actionactionChange_Default_Web_Browser_triggered()
{
    ConfigManager::updateBrowser(this);
}

void TranscriptReviewer::on_actionAlways_On_Top_triggered(bool checked)
{
    statusBarManager->toggleAlwaysOnTop(this, checked);
}
