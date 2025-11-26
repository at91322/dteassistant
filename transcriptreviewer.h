#ifndef TRANSCRIPTREVIEWER_H
#define TRANSCRIPTREVIEWER_H

#include "statusbarmanager.h"

#include <QMainWindow>

namespace Ui {
class TranscriptReviewer;
}

class TranscriptReviewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit TranscriptReviewer(QWidget *parent = nullptr);
    ~TranscriptReviewer();

private slots:
    void on_pushButtonHome_clicked();

    void on_actionChange_Username_triggered();
    void on_actionChange_Current_Term_triggered();

private:
    Ui::TranscriptReviewer *ui;

    StatusBarManager *statusBarManager;
};

#endif // TRANSCRIPTREVIEWER_H
