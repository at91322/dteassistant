#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QString>

#ifdef _WIN32
#include <windows.h>
#endif

class WindowManager
{
public:
    // Check if a window exists by process name or window title
    static bool windowExists(const QString &processNameOrTitle);

    // Activate (bring to front) a window by process name or window title
    static bool activateWindow(const QString &processNameOrTitle);

    // Check if a specific process is running
    static bool isProcessRunning(const QString &processName);

    // Get HWND for a window by title (partial match)
    static HWND findWindowByTitle(const QString &title);

    // Get HWND for a window by process name
    static HWND findWindowByProcess(const QString &processName);

private:
#ifdef _WIN32
    // Helper for EnumWindows callback
    struct EnumWindowsData {
        QString searchString;
        HWND hwnd;
        bool findByTitle;
    };

    static BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam);
#endif
};

#endif // WINDOWMANAGER_H
