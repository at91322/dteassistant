#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>

class ConfigManager
{
public:
    // Config access
    static QString getUsernameFromConfig();
    static QString getCurrentTermFromConfig();
    static QString getNextTermFromConfig();
    static QString getPreviousTermFromConfig();

    // Term Conversion
    static QString convertTermToLongFormat(const QString &termCode); // "202630 -> Winter 2026"

    // Config updates
    static void updateUsername(QWidget *parentWidget);
    static void updateCurrentTerm(QWidget *parentWidget);
};

#endif // CONFIGMANAGER_H
