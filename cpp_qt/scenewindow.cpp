#include "scenewindow.h"
#include "ui_scenewindow.h"
#include "QPainter"

SceneWindow::SceneWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SceneWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1000, 600);
}

SceneWindow::~SceneWindow()
{
    delete ui;
}

void SceneWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QBrush brush(QColor(0, 255, 0, 125));
    painter.setBrush(brush);

    for (int i = 0; i <= MAP_WIDTH; i += 50)
        painter.drawLine(QPointF(i, 0), QPointF(i, MAP_HEIGHT));
    for (int i = 0; i <= MAP_HEIGHT; i += 50)
        painter.drawLine(QPointF(0, i), QPointF(MAP_WIDTH, i));

    for (int i = 0; i < MAP_WIDTH; i += 100)
    {
        for (int j = 0; j < MAP_HEIGHT; j += 100)
            painter.drawRect(i, j, 50, 50);
    }


    QPixmap bg(300, 300);
    ui->graphicsView->setBackgroundBrush(QBrush(bg));

    QPainter painter2(&bg);
    painter2.drawLine(0, 0, 150, 150);

    //painter2.eraseRect(0, 0, MAP_WIDTH, MAP_HEIGHT);
    painter2.drawLine(100, 20, 250, 250);
}
