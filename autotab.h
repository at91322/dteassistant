#ifndef AUTOTAB_H
#define AUTOTAB_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QObject>
#include <QTimer>
#include <QApplication>

class AutoTabFilter : public QObject
{
    Q_OBJECT

public:
    explicit AutoTabFilter(int maxLength, QObject *parent = nullptr)
        : QObject(parent), m_maxLength(maxLength) {}

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() ==QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            QLineEdit *lineEdit = qobject_cast<QLineEdit*>(obj);

            if (lineEdit) {
                // Check if a digit was pressed and max length will be reached
                QString key = keyEvent->text();
                if(!key.isEmpty() && key[0].isDigit()) {
                    // After this character is added, check if we've reached max length
                    if (lineEdit->text().length() + 1 >= m_maxLength) {
                        // Let the character be entered first, then tab
                        QTimer::singleShot(0, [lineEdit]() {
                            // Simulate Tab key press to move to next field
                            QKeyEvent tabEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
                            QApplication::sendEvent(lineEdit, &tabEvent);
                        });
                    }
                }
            }
        }
        return QObject::eventFilter(obj, event);
    }

private:
    int m_maxLength;
};

// Helper function to easily apply autot-tab to a QLineEdit
inline void setupAutoTab(QLineEdit *lineEdit, int maxLength)
{
    if(lineEdit) {
        lineEdit->setMaxLength(maxLength);
        AutoTabFilter *filter = new AutoTabFilter(maxLength, lineEdit);
        lineEdit->installEventFilter(filter);
    }
}

#endif // AUTOTAB_H

