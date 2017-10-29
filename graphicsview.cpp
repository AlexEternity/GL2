#include "graphicsview.h"

GraphicsView::GraphicsView(QWidget *parent)
    :QGraphicsView(parent)
{
    A = NULL;
    B = NULL;
    C = NULL;
}

GraphicsView::~GraphicsView()
{

}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if( A == NULL)
    {
        A = new Point();
        A->SetX(event->x());
        A->SetY(event->y());
        scene->addItem(A);
    }
    else if( B == NULL)
    {
        B = new Point();
        B->SetX(event->x());
        B->SetY(event->y());
        scene->addItem(B);
        scene->addLine(A->GetX(),A->GetY(),B->GetX(),B->GetY(),QPen(Qt::red));
    }
    else if( C == NULL)
    {
        C = new Point();
        C->SetX(event->x());
        C->SetY(event->y());
        scene->addItem(C);
        scene->addLine(B->GetX(),B->GetY(),C->GetX(),C->GetY(),QPen(Qt::red));
        scene->addLine(C->GetX(),C->GetY(),A->GetX(),A->GetY(),QPen(Qt::red));
    }
    scene->update();
}
