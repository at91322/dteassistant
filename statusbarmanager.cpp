#include "statusbarmanager.h"

#include <QCoreApplication>
#include <QInputDialog>
#include <QMessageBox>
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

void StatusBarManager::updateUsername(QWidget *parentWidget)
{
    QString currentUsername = getUsernameFromConfig();

    bool ok;
    QString newUsername = QInputDialog::getText(
        parentWidget,
        "Change Username",
        "Enter your LCC username:",
        QLineEdit::Normal,
        currentUsername,
        &ok
        );

    if (ok && !newUsername.isEmpty()) {
        // Save to config.ini
        QString configPath = QCoreApplication::applicationDirPath() + "/config.ini";
        QSettings settings(configPath, QSettings::IniFormat);
        settings.setValue("Local/User", newUsername);
        settings.sync();

        QMessageBox::information(parentWidget, "Success",
                                 QString("Username updated to: %1\n\nRestart the application for changes to take effect.").arg(newUsername));
    }
}

void StatusBarManager::updateCurrentTerm(QWidget *parentWidget)
{
    QString currentTerm = getCurrentTermFromConfig();

    bool ok;
    QString newTerm = QInputDialog::getText(
        parentWidget,
        "Change Current Term",
        "Enter the current term code (e.g., 202630):",
        QLineEdit::Normal,
        currentTerm,
        &ok
        );

    if (ok && !newTerm.isEmpty()) {
        // Validate that it's 6 digits
        if (newTerm.length() != 6 || !newTerm.toInt()) {
            QMessageBox::warning(parentWidget, "Invalid Term Code",
                                 "Term code must be exactly 6 digits (e.g., 202630)");
            return;
        }

        // Save to config.ini
        QString configPath = QCoreApplication::applicationDirPath() + "/config.ini";
        QSettings settings(configPath, QSettings::IniFormat);
        settings.setValue("TermContext/CurrentTerm", newTerm);

        // Calculate and save previous and next terms
        int term = newTerm.toInt();
        int lastTwoDigits = term % 100;  // Get last two digits (10, 20, 30, or 40)
        int previousTerm, nextTerm;

        if (lastTwoDigits == 20 || lastTwoDigits == 30) {  // Fall (20) or Winter (30)
            previousTerm = term - 10;
            nextTerm = term + 10;
        } else if (lastTwoDigits == 10) {  // Summer (10)
            previousTerm = term - 70;
            nextTerm = term + 10;
        } else if (lastTwoDigits == 40) {  // Spring (40)
            previousTerm = term - 10;
            nextTerm = term + 70;
        } else {
            QMessageBox::warning(parentWidget, "Invalid Term Code",
                                 "Term code must end in 10, 20, 30, or 40");
            return;
        }

        settings.setValue("TermContext/PreviousTerm", QString::number(previousTerm));
        settings.setValue("TermContext/NextTerm", QString::number(nextTerm));
        settings.sync();

        QMessageBox::information(parentWidget, "Success",
                                 QString("Current term updated to: %1\nPrevious term is now: %2\nNext term is now: %3\n\nRestart the application for changes to take effect.")
                                     .arg(newTerm)
                                     .arg(previousTerm)
                                     .arg(nextTerm));
    }
}

