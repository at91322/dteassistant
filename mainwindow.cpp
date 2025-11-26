#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "enterkeyhandler.h"
#include "documentationhelper.h"
#include "keyboardautomation.h"

#include <QDir>
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
    , appCheckTimer(nullptr)
    , excelStatusLabel(nullptr)
    , chromeStatusLabel(nullptr)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("logo.ico"));

    // Setup enter/return handling for all buttons
    setupEnterKeyForButtons(this);

    // Create status label for Excel/Spreadsheet tool
    excelStatusLabel = new QLabel(this);
    excelStatusLabel->setStyleSheet("QLabel { padding: 2px 5px; }");
    ui->statusbar->addWidget(excelStatusLabel);

    // Create status label for Chrome/Browser
    chromeStatusLabel = new QLabel(this);
    chromeStatusLabel ->setStyleSheet("QLabel { padding 2px 5px; }");
    ui->statusbar->addWidget(chromeStatusLabel);

    // Create timer to check application(s) status periodically
    appCheckTimer = new QTimer(this);
    connect(appCheckTimer, &QTimer::timeout, this, &MainWindow::checkApplicationStatus);
    appCheckTimer->start(2000); // 2 seconds

    // Initial check
    checkApplicationStatus();
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

bool MainWindow::isExcelRunning()
{
#ifdef _WIN32
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    bool found = false;

    // Take snapshot of all processes in the system
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return false;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process
    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return false;
    }

    // Walk through the process list
    do {
        QString processName = QString::fromWCharArray(pe32.szExeFile).toLower();
        if (processName == "excel.exe") {
            found = true;
            break;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return found;
#else
    // For non-Windows platforms, need to implement platform-specific checks
    return false;
#endif
}

bool MainWindow::isChromeRunning()
{
#ifdef _WIN32
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    bool found = false;

    // Take snapshot of all processes in the system
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return false;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process
    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return false;
    }

    // Walk through the process list
    do {
        QString processName = QString::fromWCharArray(pe32.szExeFile).toLower();
        if (processName == "chrome.exe") {
            found = true;
            break;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return found;
#else
    // For non-Windows platforms, need to implement platform-specific checks
    return false;
#endif
}

void MainWindow::checkApplicationStatus()
{
    // Check EXCEL/spreadsheet status
    if (isExcelRunning()) {
        excelStatusLabel->setText("✓ Excel is running");
        excelStatusLabel->setStyleSheet("QLabel { color: green; padding: 2px 5px; font-weight: bold; }");
    } else {
        excelStatusLabel->setText("✗ Excel is not running");
        excelStatusLabel->setStyleSheet("QLabel { color: red; padding: 2px 5px; font-weight: bold; }");
    }

    // Check CHROME/browser status
    if (isChromeRunning()) {
        chromeStatusLabel->setText("✓ Chrome is running");
        chromeStatusLabel->setStyleSheet("QLabel { color: green; padding: 2px 5px; font-weight: bold; }");
    } else {
        chromeStatusLabel->setText("✗ Chrome is not running");
        chromeStatusLabel->setStyleSheet("QLabel { color: red; padding: 2px 5px; font-weight: bold; }");
    }
}

QString MainWindow::getUsernameFromConfig()
{
    // Assuming config.ini is in the application directory
    QString configPath = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings settings(configPath, QSettings::IniFormat);

    return settings.value("Local/User", "").toString();
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
    QString username = getUsernameFromConfig();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "DTE Assistant", "Username not found in config.ini");
        return;
    }

    QString lastName = formatLastName(username);

    // Give user time to switch to the browser window
    QMessageBox::information(this, "Case Grabber",
                             QString("Click OK, then switch to the Navigate360 window.\n\nThe automation will start in 2 seconds for: %1").arg(lastName));

    // Wait for user to switch windows
    KeyboardAutomation::wait(2000);

    // Mimic dte.ahk behavior
    KeyboardAutomation::sendTab(2, 200);
    KeyboardAutomation::sendText(lastName, 200);
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

