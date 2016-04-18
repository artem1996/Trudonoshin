#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qpainter.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdio>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->buttonGo, SIGNAL(clicked()), this, SLOT(go_out()));
    shara = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this); // Создаём объект отрисовщика
    // Устанавливаем кисть абриса
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    QColor col;
    if(shara != NULL) {
        char minT[10], maxT[10];
        sprintf(minT, "%.3f", minTemp);
        sprintf(maxT, "%.3f", maxTemp);
        ui -> minT -> setText(minT);
        ui -> maxT -> setText(maxT);
        int rangeX = 480 / dx;
        int rangeY = 120 / dy;
        int allX = rangeX * dx;
        int allY = rangeY * dy;
        double rangeTemp = maxTemp - minTemp;
        painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
        col.setRgb(255, 0, 0);
        painter.fillRect(345, 56, 10, 10, col);
        col.setRgb(0, 0, 255);
        painter.fillRect(445, 56, 10, 10, col);
        for(int i = 0; i < allX; i++)
            for(int j = 0; j < allY; j++) {
                int qX = i / rangeX;
                int qY = j / rangeY;
                double pY = (double)(j % rangeY) / rangeY;
                double pX = (double)(i % rangeX) / rangeX;
                double temp = 1.061 * sqrt(pY * pY + pX * pX);
                double temper = shara[qX][qY] * (1 - (temp > 1 ? 1 : temp));
                temp = 1.061 * sqrt((pX * pX) + (1 - pY) * (1 - pY));
                temper += shara[qX][qY + 1] * (1 - (temp > 1 ? 1 : temp));
                temp = 1.061 * sqrt((1 - pX) * (1 - pX) + pY * pY);
                temper += shara[qX + 1][qY] * (1 - (temp > 1 ? 1 : temp));
                temp = 1.061 * sqrt((1 - pX) * (1 - pX) + (1 - pY) * (1 - pY));
                temper += shara[qX + 1][qY + 1] * (1 - (temp > 1 ? 1 : temp));
                temper -= minTemp;
                double perc = temper / rangeTemp;
                if (perc < 0) perc = 0;
                col.setRgb(perc * 255, 0, 255 * (1 - perc));
                painter.setPen(col);
                painter.drawPoint(i + 30, j + 80);
            }
    }
}

void MainWindow::go_out()
{
    static int counter;
    static double a, dt;
    if(!counter) {
        std::stringstream ss;
        ss << ui->dT->text().toUtf8().constData() << "\n";
        ss >> dt;
        ss << ui->kA->text().toUtf8().constData();
        ss >> a;
        dx = atoi(ui->dX->text().toUtf8().constData());
        dy = atoi(ui->dY->text().toUtf8().constData());
        if(dt > 0 && dx > 2 && dy > 2 && a > 0) {
            ui->dT->setReadOnly(1);
            ui->dX->setReadOnly(1);
            ui->dY->setReadOnly(1);
            ui->kA->setReadOnly(1);
            ui->lErr->setText("Начнём, пожалуй! Начальное состояние.");
            ui->buttonGo->setText("Итерация!");
            meth = new noExp(dx, dy, dt, a);
            shara = meth->share(minTemp, maxTemp);
            repaint();
            meth->sharePrint(shara);
            shara = NULL;
            counter++;
            return;
        } else {
            ui->lErr->setText("Что-то здесь не так... Может оплавились провода?");
            return;
        }
    }
    double time = counter * dt;
    static int to_exit = 0;
    if(!to_exit) {
        tempSolution = meth->iteration();
        char out3[60];
        sprintf(out3, "Итерация № %d; Время %f", counter, time);
        ui->lErr->setText(out3);
        counter++;
        shara = meth->share(minTemp, maxTemp);
        repaint();
        meth -> sharePrint(shara);
        shara = NULL;
    }
    if(time >= 20) {
        to_exit++;
        ui->buttonGo->setText("Выход");
        if(to_exit > 1)
            exit(0);
    }
}
