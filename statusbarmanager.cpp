#include "statusbarmanager.h"
#include <QCoreApplication>
#include <QSettings>

#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#endif

StatusBarManager::StatusBarManager(QStatusBar *statusBar, QObject *parent)
    : QObject(parent)
    , m_statusBar(statusBar)
    , m_appCheckTimer(nullptr)
    , m_excelStatusLabel(nullptr)
    , m_chromeStatusLabel(nullptr)
    , m_usernameLabel(nullptr)
    , m_currentTermLabel(nullptr)
{
    setupStatusBar();
}

StatusBarManager::~StatusBarManager()
{
    if (m_appCheckTimer) {
        m_appCheckTimer->stop();
    }
}

void StatusBarManager::setupStatusBar()
{
    // Remove borders from status bar items
    m_statusBar->setStyleSheet("QStatusBar::item { border: none; }");

    // Create status label for Excel
    m_excelStatusLabel = new QLabel(m_statusBar);
    m_excelStatusLabel->setStyleSheet("QLabel { padding: 2px 5px; border: none; }");
    m_statusBar->addWidget(m_excelStatusLabel);

    // Create status label for Chrome
    m_chromeStatusLabel = new QLabel(m_statusBar);
    m_chromeStatusLabel->setStyleSheet("QLabel { padding: 2px 5px; border: none; }");
    m_statusBar->addWidget(m_chromeStatusLabel);

    // Create status label for username
    QString username = getUsernameFromConfig();
    m_usernameLabel = new QLabel(m_statusBar);
    m_usernameLabel->setText(QString("User: %1").arg(username.isEmpty() ? "Not set" : username));
    m_usernameLabel->setStyleSheet("QLabel { padding: 2px 5px; border: none; }");
    m_statusBar->addPermanentWidget(m_usernameLabel);

    // Create status label for current term
    QString currentTerm = getCurrentTermFromConfig();
    m_currentTermLabel = new QLabel(m_statusBar);
    m_currentTermLabel->setText(QString("Term: %1").arg(currentTerm));
    m_currentTermLabel->setStyleSheet("QLabel { padding: 2px 5px; border: none; }");
    m_statusBar->addPermanentWidget(m_currentTermLabel);

    // Create and start timer
    m_appCheckTimer = new QTimer(this);
    connect(m_appCheckTimer, &QTimer::timeout, this, &StatusBarManager::checkApplicationStatus);

    // Initial check
    checkApplicationStatus();
}

void StatusBarManager::startMonitoring()
{
    if (m_appCheckTimer) {
        m_appCheckTimer->start(2000); // Check every 2 seconds
    }
}

void StatusBarManager::stopMonitoring()
{
    if (m_appCheckTimer) {
        m_appCheckTimer->stop();
    }
}

void StatusBarManager::checkApplicationStatus()
{
    // Check Excel status
    if (isExcelRunning()) {
        m_excelStatusLabel->setText("✓ Excel is running");
        m_excelStatusLabel->setStyleSheet("QLabel { color: green; padding: 2px 5px; font-weight: bold; border: none; }");
    } else {
        m_excelStatusLabel->setText("✗ Excel is not running");
        m_excelStatusLabel->setStyleSheet("QLabel { color: red; padding: 2px 5px; font-weight: bold; border: none; }");
    }

    // Check Chrome status
    if (isChromeRunning()) {
        m_chromeStatusLabel->setText("✓ Chrome is running");
        m_chromeStatusLabel->setStyleSheet("QLabel { color: green; padding: 2px 5px; font-weight: bold; border: none; }");
    } else {
        m_chromeStatusLabel->setText("✗ Chrome is not running");
        m_chromeStatusLabel->setStyleSheet("QLabel { color: red; padding: 2px 5px; font-weight: bold; border: none; }");
    }
}

bool StatusBarManager::isExcelRunning()
{
#ifdef _WIN32
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    bool found = false;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return false;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return false;
    }

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
    return false;
#endif
}

bool StatusBarManager::isChromeRunning()
{
#ifdef _WIN32
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    bool found = false;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return false;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return false;
    }

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
    return false;
#endif
}

QString StatusBarManager::getUsernameFromConfig()
{
    QString configPath = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings settings(configPath, QSettings::IniFormat);
    return settings.value("Local/User", "").toString();
}

QString StatusBarManager::getCurrentTermFromConfig()
{
    QString configPath = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings settings(configPath, QSettings::IniFormat);
    return settings.value("TermContext/CurrentTerm", "Not set").toString();
}

