#include "keyboardautomation.h"

#ifdef _WIN32

void KeyboardAutomation::sendKey(int key, int delayMs)
{
    keybd_event(key, 0, 0, 0);
    QThread::msleep(50);
    keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
    QThread::msleep(delayMs);
}

void KeyboardAutomation::sendText(const QString &text, int delayMs)
{
    for (int i = 0; i < text.length(); i++) {
        sendChar(text[i]);
        QThread::msleep(delayMs);
    }
}

void KeyboardAutomation::sendChar(QChar ch)
{
    // Convert character to virtual key code and send
    SHORT vk = VkKeyScan(ch.unicode());

    if (vk == -1) return; // Character can't be typed

    BYTE keyCode = LOBYTE(vk);
    BYTE shiftState = HIBYTE(vk);

    // Press shift if needed
    if (shiftState & 1) {
        keybd_event(VK_SHIFT, 0, 0, 0);
    }

    // Press the key
    keybd_event(keyCode, 0, 0, 0);
    QThread::msleep(50);
    keybd_event(keyCode, 0, KEYEVENTF_KEYUP, 0);

    // Release shift if needed
    if (shiftState & 1) {
        keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
    }
}

void KeyboardAutomation::sendTab(int count, int delayMs)
{
    for (int i = 0; i < count; i++) {
        sendKey(VK_TAB, delayMs);
    }
}

void KeyboardAutomation::sendEnter(int delayMs)
{
    sendKey(VK_RETURN, delayMs);
}

void KeyboardAutomation::wait(int milliseconds)
{
    QThread::msleep(milliseconds);
}

#endif // _WIN32
