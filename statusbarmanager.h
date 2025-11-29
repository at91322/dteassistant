#ifndef STATUSBARMANAGER_H
#define STATUSBARMANAGER_H

#include <QObject>
#include <QStatusBar>
#include <QLabel>
#include <QTimer>
#include <QFrame>

class StatusBarManager : public QObject
{
    Q_OBJECT

public:
    explicit StatusBarManager(QStatusBar *statusBar, QObject *parent = nullptr);
    ~StatusBarManager();

    // Start/stop monitoring
    void startMonitoring();
    void stopMonitoring();

    // Window management
    void toggleAlwaysOnTop(QWidget *window, bool checked);

    // Config wrappers for convenience (delegate to ConfigManager)
    void updateUsername(QWidget *parentWidget);
    void updateCurrentTerm(QWidget *parentWidget);
    void updateBrowser(QWidget *parentWidget);
    QString getCurrentTermFromConfig();
    QString getNextTermFromConfig();
    QString getBrowserFromConfig();
    QString convertTermToLongFormat(const QString &termCode);

private slots:
    void checkApplicationStatus();

private:
    QStatusBar *m_statusBar;
    QTimer *m_appCheckTimer;

    // Status labels
    QLabel *m_excelStatusLabel;
    QLabel *m_browserStatusLabel;
    QLabel *m_usernameLabel;
    QLabel *m_currentTermLabel;

    // Helper methods
    bool isExcelRunning();
    bool isBrowserRunning();

    void setupStatusBar();
};

#endif // STATUSBARMANAGER_H
