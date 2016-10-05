#include "mainwindow.h"
#include "ui_mainwindow.h"

#define ONETWO 2
#define RESOLUTION 480

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
    if(shara != NULL) {
        double rangeTemp = maxTemp - minTemp;
        char minT[10], maxT[10];
        sprintf(minT, "%.3f", minTemp);
        sprintf(maxT, "%.3f", maxTemp);
        ui -> minT -> setText(minT);
        ui -> maxT -> setText(maxT);
        int r = RESOLUTION / 2 * 0.75;
        int R0 = RESOLUTION / 2;
        int lenDR = (R0 - r) / dr;
        double NetStep = 2 * M_PI / dfi;
        R0 = r + lenDR * dr;
        QColor color;

        for (int rr = r; rr < R0; rr++) {
            int tempR = 2 * M_PI * rr;
            double fiStep = 2 * M_PI / tempR;
            for(double fi = 0; fi <= 2 * M_PI; fi += fiStep) {
                int qX = (rr - r) / lenDR;                               //Радиус-ветор
                int qY = fi / NetStep;                              //угол
                double pX = (double)((rr-r) % lenDR) / lenDR;
                double pY = (fi / NetStep) - (int)(fi / NetStep);

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
                int x = cos(fi - M_PI / 6) * rr + R0;
                int y = sin(fi - M_PI / 6) * rr + R0;
                color.setRgb(R * 255, G * 255, B * 255);
                painter.setPen(color);
                painter.drawPoint(x + 30, y + 80);
            }
        }
    }
}

void MainWindow::go_out()
{
    static int counter;
    static MKR* answer1;
    static MKR* answer2;
    if(!counter) {
        dr = atoi(ui->dX->text().toUtf8().constData());
        dfi = atoi(ui->dY->text().toUtf8().constData());
        if(1) {
            ui->dX->setReadOnly(1);
            ui->dY->setReadOnly(1);
            ui->lErr->setText("Начнём, пожалуй! Начальное состояние.");
            ui->buttonGo->setText("Расчёт!");
            answer1 = step();
            dr *= ONETWO;
            dfi *= ONETWO;
            counter++;
            return;
        } else {
            ui->lErr->setText("Что-то здесь не так... Может оплавились провода?");
            return;
        }
    }
    if(counter == 1) {
        answer2 = step();
        dr /= ONETWO;
        dfi /= ONETWO;
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
    meth = new MKR(dr, dfi);
    char out3[60];
    if(meth->iteration() == NULL) {
        sprintf(out3, "Усё пропало, Шеф!");
        ui->lErr->setText(out3);
        return NULL;
    } else {
        sprintf(out3, "Решение c количеством шагов по OX = %d, OY = %d", dr, dfi);
        ui->lErr->setText(out3);
    }
    shara = meth->share(minTemp, maxTemp);
    repaint();
    meth->sharePrint(shara);
    shara = NULL;
    return meth;
}
