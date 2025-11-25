#ifndef ENTERKEYHANDLER_H
#define ENTERKEYHANDLER_H

#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include <QPushButton>
#include <QWidget>

class EnterKeyHandler : public QObject
{
    Q_OBJECT

public:
    explicit EnterKeyHandler(QWidget *parent = nullptr)
        : QObject(parent) {}

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

            // Check for Enter or Return key
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                // Check if a QPushButton has focus
                QPushButton *button = qobject_cast<QPushButton*>(obj);
                if (button) {
                    // Trigger the button click
                    button->click();
                    return true; // event handled
                }
            }
        }
        return QObject::eventFilter(obj, event);
    }
};

// Helper function to install the handler on all buttons in a widget
inline void setupEnterKeyForButtons(QWidget *parent)
{
    if (!parent) return;

    EnterKeyHandler *handler = new EnterKeyHandler(parent);

    // Find all QPushButton children and install the event filter
    QList<QPushButton*> buttons = parent->findChildren<QPushButton*>();
    for (QPushButton *button : buttons) {
        button ->installEventFilter(handler);
    }
}

#endif // ENTERKEYHANDLER_H
