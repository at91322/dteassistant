#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QWidget>

class ConfigManager
{
public:
    // Config access
    static QString getUsernameFromConfig();
    static QString getCurrentTermFromConfig();
    static QString getNextTermFromConfig();
    static QString getPreviousTermFromConfig();
    static QString getBrowserFromConfig();

    // Term Conversion
    static QString convertTermToLongFormat(const QString &termCode); // "202630 -> Winter 2026"

    // Config updates
    static void updateUsername(QWidget *parentWidget);
    static void updateCurrentTerm(QWidget *parentWidget);
    static void updateBrowser(QWidget *parentWidget);

    // Browser helper
    static QString getBrowserDisplayName(const QString &browserExe);
};

#endif // CONFIGMANAGER_H
