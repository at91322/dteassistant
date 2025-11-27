#ifndef PROGRAMCHANGER_H
#define PROGRAMCHANGER_H

#include "statusbarmanager.h"

#include <QMainWindow>

namespace Ui {
class ProgramChanger;
}

class ProgramChanger : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProgramChanger(QWidget *parent = nullptr);
    ~ProgramChanger();

private slots:
    void on_pushButtonHome_clicked();

    void on_actionChange_Username_triggered();
    void on_actionChange_Current_Term_triggered();
    void on_actionAlways_On_Top_triggered(bool checked);

    void on_pushButton_StandardSingleToSingle_clicked();

    void on_pushButton_StandardSingleToStacked_clicked();

    void on_pushButton_StandardStackedToSingle_clicked();

    void on_pushButton_StandardStackedToStacked_clicked();

    void on_pushButton_StandardRemove_clicked();

    void on_pushButton_StandardAdd_clicked();

    void on_pushButton_StandardUpdate_clicked();

    void on_pushButton_UrgentSingleToSingle_clicked();

    void on_pushButton_UrgentSingleToStacked_clicked();

    void on_pushButton_UrgentStackedToSingle_clicked();

    void on_pushButton_UrgentStackedToStacked_clicked();

    void on_pushButton_UrgentRemove_clicked();

    void on_pushButton_UrgentAdd_clicked();

    void on_pushButton_UrgentUpdate_clicked();

private:
    Ui::ProgramChanger *ui;

    StatusBarManager *statusBarManager;
};

#endif // PROGRAMCHANGER_H
