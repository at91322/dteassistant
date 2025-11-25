#ifndef TRANSCRIPTREVIEWER_H
#define TRANSCRIPTREVIEWER_H

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

private:
    Ui::TranscriptReviewer *ui;
};

#endif // TRANSCRIPTREVIEWER_H
