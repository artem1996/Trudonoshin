#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qpainter.h"
#include <iostream>
#include <string>
#include <cmath>
#include <cstdio>
#include "MKR.h"

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
    MKR *meth;
    int dx, dy;
    double minTemp, maxTemp;
    double **shara;

public slots:
    void go_out();
    MKR* step();
};

#endif // MAINWINDOW_H
