#include "point.h"

Point::Point(QObject *parent) : QObject(parent)
{

}

Point::~Point()
{

}

void Point::SetX(double x)
{
    this->x = x;
}

void Point::SetY(double y)
{
    this->y = y;
}

double Point::GetX()
{
    return this->x;
}

double Point::GetY()
{
    return this->y;
}

QRectF Point::boundingRect() const
{
    return QRectF(QPointF(0.,0.), QSizeF(1., 1.));
}

void Point::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::red);
    painter->drawEllipse(QPointF(x,y),5,5);
}
