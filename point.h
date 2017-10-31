#ifndef POINT_H
#define POINT_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>

class Point : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Point(QObject *parent = nullptr);
    ~Point();

    void SetX(double x);
    void SetY(double x);
    double GetX();
    double GetY();
    bool move;
signals:

public slots:

private:
    double x,y;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // POINT_H
