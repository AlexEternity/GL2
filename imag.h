#ifndef IMAG_H
#define IMAG_H

#include <QImage>
#include <QPaintEvent>

class Imag: public QImage
{
public:
    Imag();
    QImage *img;
    double x,y,i,j;
    QColor a;
protected:
    void paintEvent(QPaintEvent *);
};

#endif // IMAG_H
