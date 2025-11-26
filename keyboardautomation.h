#ifndef KEYBOARDAUTOMATION_H
#define KEYBOARDAUTOMATION_H

#include <QString>
#include <QThread>

#ifdef _WIN32
#include <windows.h>
#endif

class KeyboardAutomation
{
public:
    // Send a single key press
    static void sendKey(int key, int delayMs = 100);

    // Send text (types out a string)
    static void sendText(const QString &text, int delayMs = 100);

    // Send Tab key
    static void sendTab(int count = 1, int delayMs = 100);

    // Send Enter/Return key
    static void sendEnter(int delayMs = 100);

    // Wait/Sleep
    static void wait(int milliseconds);

private:
    // Helper to convert QString to key events
    static void sendChar(QChar ch);
};

#endif // KEYBOARDAUTOMATION_H
