#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>

class ConfigManager : public QObject
{
    Q_OBJECT

public:
    explicit ConfigManager(QObject *parent = nullptr);
    ~ConfigManager();

    // Config access
    QString getUsernameFromConfig();
    QString getCurrentTermFromConfig();
    QString getNextTermFromConfig();
    QString getPreviousTermFromConfig();

    // Term Conversion
    QString convertTermToLongFormat(const QString &termCode); // "202630 -> Winter 2026"

    // Config updates
    void updateUsername(QWidget *parentWidget);
    void updateCurrentTerm(QWidget *parentWidget);

private:
};

#endif // CONFIGMANAGER_H
