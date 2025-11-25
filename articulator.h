#ifndef ARTICULATOR_H
#define ARTICULATOR_H

#include <QMainWindow>

namespace Ui {
class Articulator;
}

class Articulator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Articulator(QWidget *parent = nullptr);
    ~Articulator();

private slots:
    void on_pushButtonHome_clicked();

private:
    Ui::Articulator *ui;
};

#endif // ARTICULATOR_H
