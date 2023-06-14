
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScreen>
#include <QColorDialog>
#include <QPushButton>
#include <QPalette>
#include <QString>
#include <QPointF>
#include <QPoint>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include <QPainter>
#include <QLabel>
#include <QPen>
#include <QPixmap>
#include <QCursor>
#include <QImage>
#include <QStack>

QString s = "l";
int i = 0;
QPoint points[4];

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(640,640);
    ui->shapetext->move(10,10);
    ui->shapetext->setFixedHeight(ui->color->height());
    ui->shapeselector->move( ui->shapetext->width(),ui->shapetext->pos().y());
    ui->colortext->move(100 + ui->shapeselector->pos().x() + ui->shapeselector->width() + 10, ui->shapeselector->pos().y());
    ui->colortext->setFixedHeight(ui->color->height());
    ui->color->move(ui->colortext->pos().x() + ui->colortext->width() - 10, ui->colortext->pos().y());
    ui->paintView->setFixedSize(this->size().width() - 20,this->height() - 20 - ui->shapeselector->height());
    ui->paintView->move(10,20 + ui->shapeselector->height());
    ui->clear->move(this->size().width() - ui->clear->width() - 10, ui->color->pos().y());
    ui->fill->move(ui->color->pos().x() + ui->color->width() + 5,ui->color->pos().y());
    QString style = QString("background-color: %1").arg("black");
    ui->color->setStyleSheet(style);
    ui->paintView->setStyleSheet("background-color: white;");
    QPixmap pixmap(ui->paintView->size());
    pixmap.fill(Qt::white);
    ui->paintView->setPixmap(pixmap);
    connect(ui->color,SIGNAL(released()),this,SLOT(openColorDialog()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::openColorDialog(){
    QColorDialog dialog(this);
    QPushButton *culoare = qobject_cast<QPushButton*>(sender());
    if(dialog.exec() == QDialog::Accepted)
    {
        QColor color = dialog.selectedColor();
        QPalette palette = culoare->palette();
        palette.setColor(QPalette::Button,color);
        culoare->setPalette(palette);
        QString style = QString("background-color: %1").arg(color.name());
        culoare->setStyleSheet(style);
    }

}



void MainWindow::on_shapeselector_currentIndexChanged(int index)
{
    if(index == 0){
        s = 'l';
    }else if(index == 1){
        s = "t";
    }else if(index == 2){
        s = "q";
    }else if(index == 3){
        s = "c";
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    QPixmap pixmap = ui->paintView->pixmap().copy();
    QPainter painter(&pixmap);
    QPen pen(ui->color->palette().color(QPalette::Button));
    painter.setPen(pen);
    if (event->button() == Qt::LeftButton && ui->paintView->underMouse()){
        points[i] = ui->paintView->mapToParent(event->pos());
        points[i].setX(points[i].x() - 22);
        points[i].setY(points[i].y() - 97);
        i++;
        if(ui->paintView->cursor() == Qt::ArrowCursor){
            if(s=="l" && i==2)
            {
                i = 0;
                painter.drawLine(points[0],points[1]);
                ui->paintView->setPixmap(pixmap);
            }else if(s=="t" && i==3){
                i=0;
                painter.drawLine(points[0],points[1]);
                painter.drawLine(points[1],points[2]);
                painter.drawLine(points[2],points[0]);
                ui->paintView->setPixmap(pixmap);
            }else if(s=="q" && i==2){
                i=0;
                QPoint p(points[1].x(),points[0].y());
                QPoint q(points[0].x(),points[1].y());
                painter.drawLine(points[0],p);
                painter.drawLine(p,points[1]);
                painter.drawLine(points[1],q);
                painter.drawLine(q,points[0]);
                ui->paintView->setPixmap(pixmap);
            }else if(s=="c" && i==2){
                i=0;
                qreal radius = sqrt(pow(points[0].x()-points[1].x(),2) + pow(points[0].y()-points[1].y(),2));
                painter.drawEllipse(points[0],radius,radius);
                ui->paintView->setPixmap(pixmap);
            }
        }else{
            i=0;
            QImage image = ui->paintView->pixmap().toImage();
            FillRecursiv(image,points[0]);

        }
    }
}


void MainWindow::on_clear_released()
{
    QPixmap p(ui->paintView->size());
    p.fill(Qt::white);
    ui->paintView->setPixmap(p);
}


void MainWindow::on_fill_released()
{
    if(ui->paintView->cursor() == Qt::ArrowCursor)
    {
        QPixmap pixmap("D:/Projects/Qt Projects/DrawingThings/resources/bucket.png");
        QCursor cursor(pixmap.scaled(20,20));
        ui->paintView->setCursor(cursor);
    }else{
        ui->paintView->setCursor(Qt::ArrowCursor);
    }
}

void MainWindow::FillRecursiv(QImage image, QPoint point){
    QStack<QPoint> pointlist;
    pointlist.push(point);
    QColor cul = image.pixelColor(point);
    while(!pointlist.isEmpty()){
       QPoint pixel = pointlist.pop();
        if(image.pixelColor(pixel) == cul){
            image.setPixelColor(pixel,ui->color->palette().color(QPalette::Button));
            if (pixel.x() > 0) {
                pointlist.push(QPoint(pixel.x() - 1, pixel.y()));
            }
            if (pixel.x() < image.width() - 1) {
                pointlist.push(QPoint(pixel.x() + 1, pixel.y()));
            }
            if (pixel.y() > 0) {
                pointlist.push(QPoint(pixel.x(), pixel.y() - 1));
            }
            if (pixel.y() < image.height() - 1) {
                pointlist.push(QPoint(pixel.x(), pixel.y() + 1));
            }
            QPixmap pixmap = QPixmap::fromImage(image);
            ui->paintView->setPixmap(pixmap);
       }
    }
}

