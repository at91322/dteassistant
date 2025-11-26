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

    // Config access
    QString getUsernameFromConfig();
    QString getCurrentTermFromConfig();

    // Config updates
    void updateUsername(QWidget *parentWidget);
    void updateCurrentTerm(QWidget *parentWidget);

    // Window management
    void toggleAlwaysOnTop(QWidget *window, bool checked);

private slots:
    void checkApplicationStatus();

private:
    QStatusBar *m_statusBar;
    QTimer *m_appCheckTimer;

    // Status labels
    QLabel *m_excelStatusLabel;
    QLabel *m_chromeStatusLabel;
    QLabel *m_usernameLabel;
    QLabel *m_currentTermLabel;

    // Helper methods
    bool isExcelRunning();
    bool isChromeRunning();

    void setupStatusBar();
};

#endif // STATUSBARMANAGER_H
