#include "configmanager.h"

#include <QCoreApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>

#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#endif

QString ConfigManager::getUsernameFromConfig()
{
    QString configPath = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings settings(configPath, QSettings::IniFormat);
    return settings.value("Local/User", "").toString();
}

QString ConfigManager::getCurrentTermFromConfig()
{
    QString configPath = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings settings(configPath, QSettings::IniFormat);
    return settings.value("TermContext/CurrentTerm", "Not set").toString();
}

QString ConfigManager::getNextTermFromConfig()
{
    QString configPath = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings settings(configPath, QSettings::IniFormat);
    return settings.value("TermContext/NextTerm", "Not set").toString();
}

QString ConfigManager::getPreviousTermFromConfig()
{
    QString configPath = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings settings(configPath, QSettings::IniFormat);
    return settings.value("TermContext/PreviousTerm", "Not set").toString();
}

QString ConfigManager::getBrowserFromConfig()
{
    QString configPath = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings settings(configPath, QSettings::IniFormat);
    return settings.value("Local/Browser", "chrome.exe").toString();
}

QString ConfigManager::convertTermToLongFormat(const QString &termCode)
{
    if(termCode.length() != 6) {
        return termCode; // Invalid format, return as is
    }

    QString year = termCode.left(4);
    QString termPart = termCode.right(2);

    QString termName;
    int displayYear = year.toInt();

    if (termPart == "10") {
        termName = "Summer";
        displayYear = year.toInt() - 1;
    } else if (termPart == "20") {
        termName = "Fall";
        displayYear = year.toInt() - 1;
    } else if (termPart == "30") {
        termName = "Winter";
        displayYear = year.toInt();
    } else if (termPart == "40") {
        termName = "Spring";
        displayYear = year.toInt();
    } else {
        return termCode; //Unknown term code
    }

    return QString("%1 %2").arg(termName).arg(displayYear);
}

QString ConfigManager::getBrowserDisplayName(const QString &browserExe)
{
    QString lower = browserExe.toLower();

    if (lower == "chrome.exe") return "Chrome";
    if (lower == "firefox.exe") return "Firefox";
    if (lower == "msedge.exe") return "Edge";
    if (lower == "opera.exe") return "Opera";

    return browserExe; // Return as-is if unknown
}

void ConfigManager::updateUsername(QWidget *parentWidget)
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

void ConfigManager::updateCurrentTerm(QWidget *parentWidget)
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

void ConfigManager::updateBrowser(QWidget *parentWidget)
{
    QString currentBrowser = getBrowserFromConfig();

    QStringList browsers;
    browsers << "chrome.exe" << "firefox.exe" << "msedge.exe" << "opera.exe";

    QStringList displayNames;
    for (const auto &browser : browsers) {
        displayNames << getBrowserDisplayName(browser);
    }

    // Find current selection index
    int currentIndex = browsers.indexOf(currentBrowser);
    if (currentIndex == -1) currentIndex = 0;

    bool ok;
    QString selectedDisplay = QInputDialog::getItem(
        parentWidget,
        "Change Browser",
        "Select your preferred web browser:",
        displayNames,
        currentIndex,
        false,
        &ok
        );

    if (ok) {
        // Convert display name back to executable name
        int selectedIndex = displayNames.indexOf(selectedDisplay);
        QString newBrowser = browsers[selectedIndex];

        // Save to config.ini
        QString configPath = QCoreApplication::applicationDirPath() + "/config.ini";
        QSettings settings(configPath, QSettings::IniFormat);
        settings.setValue("Local/Browser", newBrowser);
        settings.sync();

        QMessageBox::information(parentWidget, "Success",
                                 QString("Browser updated to: %1\n\nRestart the application for changes to take effect.")
                                     .arg(selectedDisplay));
    }
}

