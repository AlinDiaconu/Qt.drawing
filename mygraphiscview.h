
#ifndef MYGRAPHISCVIEW_H
#define MYGRAPHISCVIEW_H
#include <QGraphicsView>



class MyGraphiscView : public QGraphicsView
{
public:
    MyGraphiscView(QWidget* parent = nullptr) : QGraphicsView(parent){
        connect();
    }
};

#endif // MYGRAPHISCVIEW_H
