#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "noexp.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;
    noExp *meth;
    double *tempSolution;
    int dx, dy;
    double minTemp, maxTemp;
    double **shara;

public slots:
    void go_out();
};

#endif // MAINWINDOW_H
