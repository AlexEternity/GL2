#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QToolBar>
#include <QFileDialog>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <graphicsview.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void signal1(QString f);

public slots:
    void on_button_clicked();
    void on_transform_clicked();

protected:
    void paintEvent(QPaintEvent *);

private:
    GraphicsView *fWidget;
    GraphicsView *sWidget;
    QImage *image;
    QImage *image1;

    bool Om;
    bool Tm;
    QString oFile;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
