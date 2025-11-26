#include "windowmanager.h"

#ifdef _WIN32
#include <tlhelp32.h>
#include <psapi.h>

bool WindowManager::windowExists(const QString &processNameOrTitle)
{
    // Try finding by process name first
    HWND hwnd = findWindowByProcess(processNameOrTitle);
    if (hwnd != nullptr) {
        return true;
    }

    // Try finding by window title
    hwnd = findWindowByTitle(processNameOrTitle);
    return (hwnd != nullptr);
}

bool WindowManager::activateWindow(const QString &processNameOrTitle)
{
    // Try finding by process name first
    HWND hwnd = findWindowByProcess(processNameOrTitle);

    // If not found, try by title
    if (hwnd == nullptr) {
        hwnd = findWindowByTitle(processNameOrTitle);
    }

    if (hwnd != nullptr) {
        // Restore if minimized
        if (IsIconic(hwnd)) {
            ShowWindow(hwnd, SW_RESTORE);
        }

        // Bring to foreground
        SetForegroundWindow(hwnd);
        return true;
    }

    return false;
}

bool WindowManager::isProcessRunning(const QString &processName)
{
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

    QString searchName = processName.toLower();
    if (!searchName.endsWith(".exe")) {
        searchName += ".exe";
    }

    do {
        QString exeName = QString::fromWCharArray(pe32.szExeFile).toLower();
        if (exeName == searchName) {
            found = true;
            break;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return found;
}

HWND WindowManager::findWindowByTitle(const QString &title)
{
    EnumWindowsData data;
    data.searchString = title.toLower();
    data.hwnd = nullptr;
    data.findByTitle = true;

    EnumWindows(enumWindowsProc, reinterpret_cast<LPARAM>(&data));

    return data.hwnd;
}

HWND WindowManager::findWindowByProcess(const QString &processName)
{
    EnumWindowsData data;
    data.searchString = processName.toLower();
    if (!data.searchString.endsWith(".exe")) {
        data.searchString += ".exe";
    }
    data.hwnd = nullptr;
    data.findByTitle = false;

    EnumWindows(enumWindowsProc, reinterpret_cast<LPARAM>(&data));

    return data.hwnd;
}

BOOL CALLBACK WindowManager::enumWindowsProc(HWND hwnd, LPARAM lParam)
{
    EnumWindowsData *data = reinterpret_cast<EnumWindowsData*>(lParam);

    // Skip invisible windows
    if (!IsWindowVisible(hwnd)) {
        return TRUE;
    }

    if (data->findByTitle) {
        // Search by window title
        wchar_t windowTitle[256];
        GetWindowTextW(hwnd, windowTitle, 256);
        QString title = QString::fromWCharArray(windowTitle).toLower();

        if (title.contains(data->searchString)) {
            data->hwnd = hwnd;
            return FALSE; // Stop enumeration
        }
    } else {
        // Search by process name
        DWORD processId;
        GetWindowThreadProcessId(hwnd, &processId);

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
        if (hProcess != nullptr) {
            wchar_t processPath[MAX_PATH];
            if (GetModuleFileNameExW(hProcess, nullptr, processPath, MAX_PATH)) {
                QString fullPath = QString::fromWCharArray(processPath).toLower();

                // Extract just the executable name
                int lastSlash = fullPath.lastIndexOf('\\');
                QString exeName = (lastSlash != -1) ? fullPath.mid(lastSlash + 1) : fullPath;

                if (exeName == data->searchString) {
                    data->hwnd = hwnd;
                    CloseHandle(hProcess);
                    return FALSE; // Stop enumeration
                }
            }
            CloseHandle(hProcess);
        }
    }

    return TRUE; // Continue enumeration
}

#endif // _WIN32

