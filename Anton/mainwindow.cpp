#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qpainter.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdio>
#define WIDTH 240


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
        int range = WIDTH / dy;
        int all = range * dy;
        double rangeTemp = maxTemp - minTemp;
        painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
        col.setRgb(255, 0, 0);
        painter.fillRect(345, 56, 10, 10, col);
        col.setRgb(0, 0, 255);
        painter.fillRect(445, 56, 10, 10, col);
        for(int j = 0; j < all; j++)
            for(int i = all - j; i <= all; i++) {
                int qX = i / range;
                int qY = j / range;
                double pX = (double)(i % range) / range;
                double pY = (double)(j % range) / range;
                double temper;
                if(pX < 0.5) {
                    if(pY < 0.5) { //A
                        temper = shara[qX][qY] * (2 - 2 * pX - pY) + shara[qX + 1][qY] * pX + shara[qX + 1][qY + 1] * pY + shara[qX][qY + 1] * pX;
                    } else { //D
                        temper = shara[qX][qY] * (1 - pY) + shara[qX + 1][qY] * pX + shara[qX + 1][qY + 1] * (1 - pY) + shara[qX][qY + 1] * (2 * pY - pX);
                    }
                } else {
                    if(pY < 0.5) { //B
                        temper = shara[qX][qY] * pY + shara[qX + 1][qY] * (1 + pX - 2 * pY) + shara[qX + 1][qY + 1] * pY + shara[qX][qY + 1] * (1 - pX);
                    } else { //C
                        temper = shara[qX][qY] * (1 - pY) + shara[qX + 1][qY] * (1 - pX) + shara[qX + 1][qY + 1] * (2 * pX - 1 + pY) + shara[qX][qY + 1] * (1 - pX);
                    }
                }
                temper /= 2;
                temper -= minTemp;
                double perc = temper / rangeTemp;
                if (perc < 0) perc = 0;
                double R,G,B;
                if(perc > 0.5) {
                    R = (perc - 0.5) * 2;
                    B = 0;
                } else {
                    B = (0.5 - perc) * 2;
                    R = 0;
                }
                if(perc > 0.25 && perc < 0.75) {
                    G = (0.75 - perc) * 2;
                } else {
                    G = 0;
                }
                col.setRgb(R * 255, G * 255, B * 255);
                //col.setRgb(perc * 255, 0, 255 * (1 - perc));
                painter.setPen(col);
                painter.drawPoint(i + 30, j + 80);
                painter.drawPoint(30 + 2 * WIDTH - i, j + 80);
            }
    }
}

void MainWindow::go_out()
{
    static int counter;
    if(!counter) {
        dy = atoi(ui->dY->text().toUtf8().constData());
        if(dy > 2) {
            ui->dY->setReadOnly(1);
            ui->lErr->setText("Начнём, пожалуй! Начальное состояние.");
            ui->buttonGo->setText("Итерация!");
            meth = new noExp(dy);
            tempSolution = meth->iteration();
            char out3[60];
            sprintf(out3, "Готово!");
            ui->lErr->setText(out3);
            counter++;
            shara = meth->share(minTemp, maxTemp);
            repaint();
            meth -> sharePrint(shara);
            shara = NULL;
            ui->buttonGo->setText("Выход");
            return;
        } else {
            ui->lErr->setText("Что-то здесь не так...");
            return;
        }
    } else
        exit(0);
}
