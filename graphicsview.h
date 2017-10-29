#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <point.h>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent = 0);
    ~GraphicsView();

    QGraphicsScene *scene;
    Point *A;
    Point *B;
    Point *C;

signals:

private slots:

private:
    void mousePressEvent(QMouseEvent *event );

};
#endif // GRAPHICSVIEW_H
