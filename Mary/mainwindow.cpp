#include "mainwindow.h"
#include "ui_mainwindow.h"

#define ONETWO 2

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
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
        int rangeY = 480 / dy;
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
                double pX = (double)(i % rangeX) / rangeX;
                double pY = (double)(j % rangeY) / rangeY;
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
                painter.setPen(col);
                painter.drawPoint(i + 30, j + 80);
            }
    }
}

void MainWindow::go_out()
{
    static int counter;
    static MKR* answer1;
    static MKR* answer2;
    if(!counter) {
        dx = atoi(ui->dX->text().toUtf8().constData());
        dy = atoi(ui->dY->text().toUtf8().constData());
        if(dx > 7 && dy > 7 && dx % 4 == 0 && dy % 8 == 0) {
            ui->dX->setReadOnly(1);
            ui->dY->setReadOnly(1);
            ui->lErr->setText("Начнём, пожалуй! Начальное состояние.");
            ui->buttonGo->setText("Расчёт!");
            answer1 = step();
            dx *= ONETWO;
            dy *= ONETWO;
            counter++;
            return;
        } else {
            ui->lErr->setText("Что-то здесь не так... Может оплавились провода?");
            return;
        }
    }
    if(counter == 1) {
        answer2 = step();
        dx /= ONETWO;
        dy /= ONETWO;
        counter++;
        return;
    }
    if(counter == 2) {
        ui->lErr->setText("Аппроксимация...");
        answer1 -> approximation(answer2, ONETWO);
        shara = answer1->share(minTemp, maxTemp);
        repaint();
        answer1 -> sharePrint(shara);
        shara = NULL;
        ui->buttonGo->setText("Выход");
        counter++;
        return;
    }
    if(counter == 3) {
        delete answer1;
        delete answer2;
        exit(0);
    }
}

MKR* MainWindow::step() {
    meth = new MKR(dx, dy);
    char out3[60];
    if(meth->iteration() == NULL) {
        sprintf(out3, "Усё пропало, Шеф!");
        ui->lErr->setText(out3);
        return NULL;
    } else {
        sprintf(out3, "Решение c количеством шагов по OX = %d, OY = %d", dx, dy);
        ui->lErr->setText(out3);
    }
    shara = meth->share(minTemp, maxTemp);
    repaint();
    meth->sharePrint(shara);
    shara = NULL;
    return meth;
}
