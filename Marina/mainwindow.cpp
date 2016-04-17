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
        int rangeX = 250 / dx;
        int rangeY = 250 / dy;
        int allX = rangeX * dx;
        int allY = rangeY * dy;
        double rangeTemp = maxTemp - minTemp;
        painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
        for(int i = 0; i < allX; i++)
            for(int j = 0; j < allY; j++) {
                int qX = i / rangeX;
                int qY = j / rangeY;
                double pY = (double)(j % rangeY) / rangeY;
                double pX = (double)(i % rangeX) / rangeX;
                double temp = sqrt(pY * pY + pX * pX);
                double temper = shara[qX][qY] * (1 - (temp > 1 ? 1 : temp));
                temp = sqrt((pX * pX) + (1 - pY) * (1 - pY));
                temper += shara[qX][qY + 1] * (1 - (temp > 1 ? 1 : temp));
                temp = sqrt((1 - pX) * (1 - pX) + pY * pY);
                temper += shara[qX + 1][qY] * (1 - (temp > 1 ? 1 : temp));
                temp = sqrt((1 - pX) * (1 - pX) + (1 - pY) * (1 - pY));
                temper += shara[qX + 1][qY + 1] * (1 - (temp > 1 ? 1 : temp));
                temper -= minTemp;
                double perc = (temper / rangeTemp) > 1 ? 1 : temper / rangeTemp;
                col.setRgb(perc * 255, 0, 255 * (1 - perc));
                painter.setPen(col);
                painter.drawPoint(i + 50, j + 100);
            }
    }
}

void MainWindow::go_out()
{
    static int counter;
    static double a, dt;
    if(!counter) {
        minTemp = 20;
        maxTemp = 205;
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
            ui->lErr->setText("Начнём, пожалуй!\nНачальное состояние.");
            ui->buttonGo->setText("Итерация!");
            meth = new noExp(dx, dy, dt, a);
            shara = meth->share();
            repaint();
            meth->sharePrint(shara);
            shara = NULL;
            counter++;
            return;
        } else {
            ui->lErr->setText("Что-то здесь не так...\nМожет оплавились провода?");
            return;
        }
    }
    double time = counter * dt;
    static int to_exit = 0;
    if(!to_exit) {
        tempSolution = meth->iteration();
        char out3[60];
        sprintf(out3, "Итерация № %d\nВремя %f\0", counter, time);
        ui->lErr->setText(out3);
        counter++;
        shara = meth->share();
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
