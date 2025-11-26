#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "articulator.h"
#include "termbuilder.h"
#include "transcriptreviewer.h"
#include "programchanger.h"
#include "diplomadates.h"

#include <QMainWindow>
#include <QTimer>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Articulate_clicked();
    void on_pushButton_TermBuilder_clicked();
    void on_pushButton_EnterCourses_clicked();
    void on_pushButton_GrabCases_clicked();
    void on_pushButton_CaseComments_clicked();
    void on_pushButton_DiplDates_clicked();

    void checkApplicationStatus();

    void on_pushButton_Help_ARTA_clicked();
    void on_pushButton_Help_CHNG_clicked();
    void on_pushButton_Help_SHADIPL_clicked();

private:
    Ui::MainWindow *ui;

    Articulator *articulatorWindow;
    TermBuilder *termBuilderWindow;
    TranscriptReviewer *transcriptReviewerWindow;
    ProgramChanger *programChangerWindow;
    DiplomaDates *diplomaDatesWindow;

    QTimer *appCheckTimer;
    QLabel *excelStatusLabel;
    QLabel *chromeStatusLabel;

    bool isExcelRunning();
    bool isChromeRunning();

    QString getUsernameFromConfig();
    QString formatLastName(const QString &username);

    QLabel *usernameLabel;
    QLabel *currentTermLabel;
};
#endif // MAINWINDOW_H
