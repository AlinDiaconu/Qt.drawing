
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void openColorDialog();
    void on_shapeselector_currentIndexChanged(int index);
    void mousePressEvent(QMouseEvent *event);
    void on_clear_released();
    void on_fill_released();
    void FillRecursiv(QImage image,QPoint point);
};

#endif // MAINWINDOW_H
