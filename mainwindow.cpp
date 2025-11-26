#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "enterkeyhandler.h"

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
    ui->statusbar->addPermanentWidget(excelStatusLabel);

    // Create status label for Chrome/Browser
    chromeStatusLabel = new QLabel(this);
    chromeStatusLabel ->setStyleSheet("QLabel { padding 2px 5px; }");
    ui->statusbar->addPermanentWidget(chromeStatusLabel);

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
        delete articulatorWindow;
    }
    if (transcriptReviewerWindow != nullptr) {
        delete articulatorWindow;
    }
    if (programChangerWindow != nullptr) {
        delete articulatorWindow;
    }
    if (diplomaDatesWindow != nullptr) {
        delete articulatorWindow;
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
