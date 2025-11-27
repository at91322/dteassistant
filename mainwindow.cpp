#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "windowmanager.h"
#include "enterkeyhandler.h"
#include "keyboardautomation.h"
#include "documentationhelper.h"

#include <QDir>
#include <QFrame>
#include <QSettings>

#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , articulatorWindow(nullptr)
    , termBuilderWindow(nullptr)
    , transcriptReviewerWindow(nullptr)
    , programChangerWindow(nullptr)
    , diplomaDatesWindow(nullptr)
    , statusBarManager(nullptr)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("logo.ico"));

    ui->statusbar->setStyleSheet("QStatusBar::item { border: none; }");

    // Setup enter/return handling for all buttons
    setupEnterKeyForButtons(this);

    // Setup status bar
    statusBarManager = new StatusBarManager(ui->statusbar, this);
    statusBarManager->startMonitoring();

    // Make the Always On Top action checkable
    ui->actionAlways_On_Top->setCheckable(true);
}

MainWindow::~MainWindow()
{
    if (articulatorWindow != nullptr) {
        delete articulatorWindow;
    }
    if (termBuilderWindow != nullptr) {
        delete termBuilderWindow;
    }
    if (transcriptReviewerWindow != nullptr) {
        delete transcriptReviewerWindow;
    }
    if (programChangerWindow != nullptr) {
        delete programChangerWindow;
    }
    if (diplomaDatesWindow != nullptr) {
        delete diplomaDatesWindow;
    }
    delete ui;
}

QString MainWindow::formatLastName(const QString &username)
{
    if (username.isEmpty()) {
        return QString();
    }

    // Convert first character to uppercase, rest to lowercase
    QString lastName = username.toUpper().left(1) + username.mid(1, username.length() - 2).toLower();

    return lastName;
}

void MainWindow::on_pushButton_Articulate_clicked()
{
    // Create the window if it does not exist
    if (articulatorWindow == nullptr) {
        articulatorWindow = new Articulator(this);
    }

    // Hide this main window
    this->hide();

    // Show the articulator window
    articulatorWindow->show();
}

void MainWindow::on_pushButton_TermBuilder_clicked()
{
    // Create; Hide; Show
    if (termBuilderWindow == nullptr) {
        termBuilderWindow = new TermBuilder(this);
    }
    this->hide();
    termBuilderWindow->show();
}

void MainWindow::on_pushButton_EnterCourses_clicked()
{
    // Create; Hide; Show
    if (transcriptReviewerWindow == nullptr) {
        transcriptReviewerWindow = new TranscriptReviewer(this);
    }
    this->hide();
    transcriptReviewerWindow->show();
}

void MainWindow::on_pushButton_GrabCases_clicked()
{
    QString username = statusBarManager->getUsernameFromConfig();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "DTE Assistant", "Username not found in config.ini");
        return;
    }

    QString lastName = formatLastName(username);

    // Activate Chrome/Browser
    if (!WindowManager::activateWindow("chrome.exe")) {
        QMessageBox::warning(this, "DTE Assistant", "Chrome window not found. Please ensure Chrome is running.", QMessageBox::Ok);
        return;
    }

    // Wait for window to activate
    KeyboardAutomation::wait(720);

    // Grab case
    KeyboardAutomation::sendTab(2, 200);
    KeyboardAutomation::sendText(lastName, 10);
    KeyboardAutomation::wait(750);
    KeyboardAutomation::sendTab(10, 200);
    KeyboardAutomation::sendEnter(500);
    KeyboardAutomation::sendTab(5, 200);
    KeyboardAutomation::sendEnter(1000);
}

void MainWindow::on_pushButton_CaseComments_clicked()
{
    // Create; Hide; Show
    if (programChangerWindow == nullptr) {
        programChangerWindow = new ProgramChanger(this);
    }
    this->hide();
    programChangerWindow->show();
}

void MainWindow::on_pushButton_DiplDates_clicked()
{
    // Create; Hide; Show
    if (diplomaDatesWindow == nullptr) {
        diplomaDatesWindow = new DiplomaDates(this);
    }
    this->hide();
    diplomaDatesWindow->show();
}

void MainWindow::on_pushButton_Help_ARTA_clicked()
{
    QString docsPath = DocumentationHelper::getDocsPath();
    QString docFile = QDir(docsPath).filePath("sample.md");
    DocumentationHelper::openDocumentation(docFile, this);
}

void MainWindow::on_pushButton_Help_CHNG_clicked()
{
    QString docsPath = DocumentationHelper::getDocsPath();
    QString docFile = QDir(docsPath).filePath("sample.md");
    DocumentationHelper::openDocumentation(docFile, this);
}

void MainWindow::on_pushButton_Help_SHADIPL_clicked()
{
    QString docsPath = DocumentationHelper::getDocsPath();
    QString docFile = QDir(docsPath).filePath("sample.md");
    DocumentationHelper::openDocumentation(docFile, this);
}

void MainWindow::on_actionChange_Username_triggered()
{
    statusBarManager->updateUsername(this);
}

void MainWindow::on_actionChange_Current_Term_triggered()
{
    statusBarManager->updateCurrentTerm(this);
}

void MainWindow::on_actionAlways_On_Top_triggered(bool checked)
{
    statusBarManager->toggleAlwaysOnTop(this, checked);
}

