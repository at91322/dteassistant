#include "programchanger.h"
#include "ui_programchanger.h"

#include "keyboardautomation.h"
#include "windowmanager.h"
#include "enterkeyhandler.h"

#include <QMessageBox>

ProgramChanger::ProgramChanger(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProgramChanger)
{
    ui->setupUi(this);

    // Setup status bar
    statusBarManager = new StatusBarManager(ui->statusbar, this);
    statusBarManager->startMonitoring();

    // Setup enter/return handling for all buttons
    setupEnterKeyForButtons(this);

    // Make the Always On Top action checkable
    ui->actionAlways_On_Top->setCheckable(true);
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

void ProgramChanger::on_actionChange_Username_triggered()
{
    statusBarManager->updateUsername(this);
}

void ProgramChanger::on_actionChange_Current_Term_triggered()
{
    statusBarManager->updateCurrentTerm(this);
}

void ProgramChanger::on_actionAlways_On_Top_triggered(bool checked)
{
    statusBarManager->toggleAlwaysOnTop(this, checked);
}


void ProgramChanger::on_pushButton_StandardSingleToSingle_clicked()
{
    QString nextTerm = statusBarManager->getNextTermFromConfig();

    if (nextTerm.isEmpty()) {
        QMessageBox::warning(this, "DTE Assistant", "Next Term not found in config.ini");
        return;
    }

    QString nextTermLongForm = statusBarManager->convertTermToLongFormat(nextTerm);

    // Activate Chrome/Browser
    if (!WindowManager::activateWindow("chrome.exe")) {
        QMessageBox::warning(this, "DTE Assistant", "Chrome window not found. Please ensure Chrome is running.", QMessageBox::Ok);
        return;
    }

    // Wait for window to activate
    KeyboardAutomation::wait(720);

    KeyboardAutomation::sendText(QString("Replaced ____ with ____ as primary program of study in %1 with same catalog term.").arg(nextTermLongForm), 1);
}


void ProgramChanger::on_pushButton_StandardSingleToStacked_clicked()
{
    QString nextTerm = statusBarManager->getNextTermFromConfig();

    if (nextTerm.isEmpty()) {
        QMessageBox::warning(this, "DTE Assistant", "Next Term not found in config.ini");
        return;
    }

    QString nextTermLongForm = statusBarManager->convertTermToLongFormat(nextTerm);

    // Activate Chrome/Browser
    if (!WindowManager::activateWindow("chrome.exe")) {
        QMessageBox::warning(this, "DTE Assistant", "Chrome window not found. Please ensure Chrome is running.", QMessageBox::Ok);
        return;
    }

    // Wait for window to activate
    KeyboardAutomation::wait(720);

    KeyboardAutomation::sendText(QString("Replaced ____ with ____ as primary program of study in %1 with same catalog term; added ____ as additional program(s) of study.").arg(nextTermLongForm), 1);
}


void ProgramChanger::on_pushButton_StandardStackedToSingle_clicked()
{
    QString nextTerm = statusBarManager->getNextTermFromConfig();

    if (nextTerm.isEmpty()) {
        QMessageBox::warning(this, "DTE Assistant", "Next Term not found in config.ini");
        return;
    }

    QString nextTermLongForm = statusBarManager->convertTermToLongFormat(nextTerm);

    // Activate Chrome/Browser
    if (!WindowManager::activateWindow("chrome.exe")) {
        QMessageBox::warning(this, "DTE Assistant", "Chrome window not found. Please ensure Chrome is running.", QMessageBox::Ok);
        return;
    }

    // Wait for window to activate
    KeyboardAutomation::wait(720);

    KeyboardAutomation::sendText(QString("Replaced ____ with ____ as primary program of study in %1 with same catalog term; removed ____ as additional program(s) of study.").arg(nextTermLongForm), 1);
}


void ProgramChanger::on_pushButton_StandardStackedToStacked_clicked()
{
    QString nextTerm = statusBarManager->getNextTermFromConfig();

    if (nextTerm.isEmpty()) {
        QMessageBox::warning(this, "DTE Assistant", "Next Term not found in config.ini");
        return;
    }

    QString nextTermLongForm = statusBarManager->convertTermToLongFormat(nextTerm);

    // Activate Chrome/Browser
    if (!WindowManager::activateWindow("chrome.exe")) {
        QMessageBox::warning(this, "DTE Assistant", "Chrome window not found. Please ensure Chrome is running.", QMessageBox::Ok);
        return;
    }

    // Wait for window to activate
    KeyboardAutomation::wait(720);

    KeyboardAutomation::sendText(QString("Replaced ____ with ____ as primary program of study in %1 with same catalog term; also replaced ____ as additional program(s) of study.").arg(nextTermLongForm), 1);

}


void ProgramChanger::on_pushButton_StandardRemove_clicked()
{
    QString nextTerm = statusBarManager->getNextTermFromConfig();

    if (nextTerm.isEmpty()) {
        QMessageBox::warning(this, "DTE Assistant", "Next Term not found in config.ini");
        return;
    }

    QString nextTermLongForm = statusBarManager->convertTermToLongFormat(nextTerm);

    // Activate Chrome/Browser
    if (!WindowManager::activateWindow("chrome.exe")) {
        QMessageBox::warning(this, "DTE Assistant", "Chrome window not found. Please ensure Chrome is running.", QMessageBox::Ok);
        return;
    }

    // Wait for window to activate
    KeyboardAutomation::wait(720);

    KeyboardAutomation::sendText(QString("Removed ____ as additional program(s) of study in %1.").arg(nextTermLongForm), 1);
}


void ProgramChanger::on_pushButton_StandardAdd_clicked()
{
    QString nextTerm = statusBarManager->getNextTermFromConfig();

    if (nextTerm.isEmpty()) {
        QMessageBox::warning(this, "DTE Assistant", "Next Term not found in config.ini");
        return;
    }

    QString nextTermLongForm = statusBarManager->convertTermToLongFormat(nextTerm);

    // Activate Chrome/Browser
    if (!WindowManager::activateWindow("chrome.exe")) {
        QMessageBox::warning(this, "DTE Assistant", "Chrome window not found. Please ensure Chrome is running.", QMessageBox::Ok);
        return;
    }

    // Wait for window to activate
    KeyboardAutomation::wait(720);

    KeyboardAutomation::sendText(QString("Added ____ as additional program(s) of study in %1.").arg(nextTermLongForm), 1);
}


void ProgramChanger::on_pushButton_StandardUpdate_clicked()
{
    QString nextTerm = statusBarManager->getNextTermFromConfig();

    if (nextTerm.isEmpty()) {
        QMessageBox::warning(this, "DTE Assistant", "Next Term not found in config.ini");
        return;
    }

    QString nextTermLongForm = statusBarManager->convertTermToLongFormat(nextTerm);

    // Activate Chrome/Browser
    if (!WindowManager::activateWindow("chrome.exe")) {
        QMessageBox::warning(this, "DTE Assistant", "Chrome window not found. Please ensure Chrome is running.", QMessageBox::Ok);
        return;
    }

    // Wait for window to activate
    KeyboardAutomation::wait(720);

    KeyboardAutomation::sendText(QString("Updated AAOT/AGS area of interest code from 19__ to 19__ in %1.").arg(nextTermLongForm), 1);
}


void ProgramChanger::on_pushButton_UrgentSingleToSingle_clicked()
{
    QString currentTerm = statusBarManager->getCurrentTermFromConfig();

    if (currentTerm.isEmpty()) {
        QMessageBox::warning(this, "DTE Assistant", "Next Term not found in config.ini");
        return;
    }

    QString currentTermLongForm = statusBarManager->convertTermToLongFormat(currentTerm);

    // Activate Chrome/Browser
    if (!WindowManager::activateWindow("chrome.exe")) {
        QMessageBox::warning(this, "DTE Assistant", "Chrome window not found. Please ensure Chrome is running.", QMessageBox::Ok);
        return;
    }

    // Wait for window to activate
    KeyboardAutomation::wait(720);

    KeyboardAutomation::sendText(QString("Replaced ____ with ____ as primary program of study in %1 with same catalog term due to URGENT request.").arg(currentTermLongForm), 1);
}


void ProgramChanger::on_pushButton_UrgentSingleToStacked_clicked()
{
    QString currentTerm = statusBarManager->getCurrentTermFromConfig();

    if (currentTerm.isEmpty()) {
        QMessageBox::warning(this, "DTE Assistant", "Next Term not found in config.ini");
        return;
    }

    QString currentTermLongForm = statusBarManager->convertTermToLongFormat(currentTerm);

    // Activate Chrome/Browser
    if (!WindowManager::activateWindow("chrome.exe")) {
        QMessageBox::warning(this, "DTE Assistant", "Chrome window not found. Please ensure Chrome is running.", QMessageBox::Ok);
        return;
    }

    // Wait for window to activate
    KeyboardAutomation::wait(720);

    KeyboardAutomation::sendText(QString("Replaced ____ with ____ as primary program of study in %1 with same catalog term due to URGENT request; added ____ as additional program(s) of study.").arg(currentTermLongForm), 1);
}


void ProgramChanger::on_pushButton_UrgentStackedToSingle_clicked()
{
    QString currentTerm = statusBarManager->getCurrentTermFromConfig();

    if (currentTerm.isEmpty()) {
        QMessageBox::warning(this, "DTE Assistant", "Next Term not found in config.ini");
        return;
    }

    QString currentTermLongForm = statusBarManager->convertTermToLongFormat(currentTerm);

    // Activate Chrome/Browser
    if (!WindowManager::activateWindow("chrome.exe")) {
        QMessageBox::warning(this, "DTE Assistant", "Chrome window not found. Please ensure Chrome is running.", QMessageBox::Ok);
        return;
    }

    // Wait for window to activate
    KeyboardAutomation::wait(720);

    KeyboardAutomation::sendText(QString("Replaced ____ with ____ as primary program of study in %1 with same catalog term due to URGENT request; removed ____ as additional program(s) of study.").arg(currentTermLongForm), 1);
}


void ProgramChanger::on_pushButton_UrgentStackedToStacked_clicked()
{
    QString currentTerm = statusBarManager->getCurrentTermFromConfig();

    if (currentTerm.isEmpty()) {
        QMessageBox::warning(this, "DTE Assistant", "Next Term not found in config.ini");
        return;
    }

    QString currentTermLongForm = statusBarManager->convertTermToLongFormat(currentTerm);

    // Activate Chrome/Browser
    if (!WindowManager::activateWindow("chrome.exe")) {
        QMessageBox::warning(this, "DTE Assistant", "Chrome window not found. Please ensure Chrome is running.", QMessageBox::Ok);
        return;
    }

    // Wait for window to activate
    KeyboardAutomation::wait(720);

    KeyboardAutomation::sendText(QString("Replaced ____ with ____ as primary program of study in %1 with same catalog term due to URGENT request; also replaced ____ as additional program(s) of study.").arg(currentTermLongForm), 1);
}


void ProgramChanger::on_pushButton_UrgentRemove_clicked()
{
    QString currentTerm = statusBarManager->getCurrentTermFromConfig();

    if (currentTerm.isEmpty()) {
        QMessageBox::warning(this, "DTE Assistant", "Next Term not found in config.ini");
        return;
    }

    QString currentTermLongForm = statusBarManager->convertTermToLongFormat(currentTerm);

    // Activate Chrome/Browser
    if (!WindowManager::activateWindow("chrome.exe")) {
        QMessageBox::warning(this, "DTE Assistant", "Chrome window not found. Please ensure Chrome is running.", QMessageBox::Ok);
        return;
    }

    // Wait for window to activate
    KeyboardAutomation::wait(720);

    KeyboardAutomation::sendText(QString("Removed ____ as additional program(s) of study in %1 due to URGENT request.").arg(currentTermLongForm), 1);
}


void ProgramChanger::on_pushButton_UrgentAdd_clicked()
{
    QString currentTerm = statusBarManager->getCurrentTermFromConfig();

    if (currentTerm.isEmpty()) {
        QMessageBox::warning(this, "DTE Assistant", "Next Term not found in config.ini");
        return;
    }

    QString currentTermLongForm = statusBarManager->convertTermToLongFormat(currentTerm);

    // Activate Chrome/Browser
    if (!WindowManager::activateWindow("chrome.exe")) {
        QMessageBox::warning(this, "DTE Assistant", "Chrome window not found. Please ensure Chrome is running.", QMessageBox::Ok);
        return;
    }

    // Wait for window to activate
    KeyboardAutomation::wait(720);

    KeyboardAutomation::sendText(QString("Added ____ as additional program(s) of study in %1 due to URGENT request.").arg(currentTermLongForm), 1);
}


void ProgramChanger::on_pushButton_UrgentUpdate_clicked()
{
    QString currentTerm = statusBarManager->getCurrentTermFromConfig();

    if (currentTerm.isEmpty()) {
        QMessageBox::warning(this, "DTE Assistant", "Next Term not found in config.ini");
        return;
    }

    QString currentTermLongForm = statusBarManager->convertTermToLongFormat(currentTerm);

    // Activate Chrome/Browser
    if (!WindowManager::activateWindow("chrome.exe")) {
        QMessageBox::warning(this, "DTE Assistant", "Chrome window not found. Please ensure Chrome is running.", QMessageBox::Ok);
        return;
    }

    // Wait for window to activate
    KeyboardAutomation::wait(720);

    KeyboardAutomation::sendText(QString("Updated AAOT/AGS area of interest code from 19__ to 19__ in %1 due to URGENT request.").arg(currentTermLongForm), 1);
}

