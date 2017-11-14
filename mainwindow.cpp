#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Om=false;
    Tm=false;
    ui->setupUi(this);
    QPalette Pal(palette());
    QToolBar *bFile=new QToolBar();
        QAction *OpenFile = bFile->addAction("OpenFile");
        QAction *Transform = bFile->addAction("Transform");
        bFile->setFixedSize(this->width(),20);
        bFile->setMovable(false);

        connect(OpenFile, SIGNAL(triggered()), SLOT(on_button_clicked()));
        connect(Transform, SIGNAL(triggered()), SLOT(on_transform_clicked()));
    addToolBar(Qt::TopToolBarArea, bFile);

    fWidget = new GraphicsView(this);
    Pal.setColor(QPalette::Background, Qt::white);
    fWidget->setAutoFillBackground(true);
    fWidget->setPalette(Pal);
    fWidget->move(0,20);
    fWidget->setFixedWidth(this->width()/2);
    fWidget->setFixedHeight(this->height());
    fWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fWidget->setAlignment(Qt::AlignCenter);                        // Делаем привязку содержимого к центру
    fWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету

    fWidget->scene = new QGraphicsScene();
    fWidget->scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    fWidget->setScene(fWidget->scene);
    fWidget->setRenderHint(QPainter::Antialiasing);
    fWidget->setCacheMode(QGraphicsView::CacheBackground);
    fWidget->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    fWidget->scene->setSceneRect(0,40,fWidget->height(),fWidget->width());
    fWidget->show();

    sWidget = new GraphicsView(this);
    Pal.setColor(QPalette::Background, Qt::white);
    sWidget->setAutoFillBackground(true);
    sWidget->setPalette(Pal);
    sWidget->setFixedWidth(this->width()/2);
    sWidget->setFixedHeight(this->height());
    sWidget->move(this->width()/2,20);
    sWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sWidget->setAlignment(Qt::AlignCenter);
    sWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sWidget->scene = new QGraphicsScene();
    sWidget->scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    sWidget->setScene(sWidget->scene);
    sWidget->setRenderHint(QPainter::Antialiasing);    // Настраиваем рендер
    sWidget->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    sWidget->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    sWidget->scene->setSceneRect(0,40,sWidget->height(),sWidget->width());
    sWidget->show();

    image = new QImage();
    image1 = new QImage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button_clicked()
{
    Om = false;
    if(oFile==0)
    {
        oFile = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.bmp *.jpg *.png");
        if (oFile.isNull())
        {
          return;
        }
    }
    if(Om == false)
    {
        Om = true;
        update();
    }
    image->load(oFile,NULL);
    //костыль
  //oFile = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.bmp *.jpg *.png");
   //image1->load(oFile,NULL);

}

void MainWindow::on_transform_clicked()
{
    fWidget->scene->update();
    sWidget->A->update();
    //исходные матрицы
    double **matr = new double*[3];
    double *b1 = new double[3];
    double *b2 = new double[3];
    for(int i = 0;i< 3;i++)
        matr[i] = new double[3];
    b1[0] = fWidget->A->GetX();
    b1[1] = fWidget->B->GetX();
    b1[2] = fWidget->C->GetX();
    b2[0] = fWidget->A->GetY();
    b2[1] = fWidget->B->GetY();
    b2[2] = fWidget->C->GetY();
    matr[0][0] = sWidget->A->GetX();
    matr[0][1] = sWidget->A->GetY();
    matr[0][2] = 1;
    matr[1][0] = sWidget->B->GetX();
    matr[1][1] = sWidget->B->GetY();
    matr[1][2] = 1;
    matr[2][0] = sWidget->C->GetX();
    matr[2][1] = sWidget->C->GetY();
    matr[2][2] = 1;

        //krummer
        double det1,det11,det12,det13;
        double det2,det21,det22,det23;
        det1 = matr[0][0]*matr[1][1]*matr[2][2]+matr[0][1]*matr[1][2]*matr[2][0]+matr[0][2]*matr[1][0]*matr[2][1]
             -(matr[0][2]*matr[1][1]*matr[2][0]+matr[0][1]*matr[1][0]*matr[2][2]+matr[0][0]*matr[1][2]*matr[2][1]);

        det11 = b1[0]*matr[1][1]*matr[2][2]+matr[0][1]*matr[1][2]*b1[2]+matr[0][2]*b1[1]*matr[2][1]
              -(matr[0][2]*matr[1][1]*b1[2]+matr[0][1]*b1[1]*matr[2][2]+b1[0]* matr[1][2]*matr[2][1]);

        det12 = matr[0][0]*b1[1]*matr[2][2]+b1[0]*matr[1][2]*matr[2][0]+matr[0][2]*matr[1][0]*b1[2]
              -(matr[0][2]*b1[1]*matr[2][0]+b1[0]*matr[1][0]*matr[2][2]+matr[0][0]*matr[1][2]*b1[2]);

        det13 = matr[0][0]*matr[1][1]*b1[2]+matr[0][1]*b1[1]*matr[2][0]+b1[0]*matr[1][0]*matr[2][1]
              -(b1[0]*matr[1][1]*matr[2][0]+matr[0][1]*matr[1][0]*b1[2]+matr[0][0]*b1[1]*matr[2][1]);

        det2 = matr[0][0]*matr[1][1]*matr[2][2]+matr[0][1]*matr[1][2]*matr[2][0]+matr[0][2]*matr[1][0]*matr[2][1]
             -(matr[0][2]*matr[1][1]*matr[2][0]+matr[0][1]*matr[1][0]*matr[2][2]+matr[0][0]*matr[1][2]*matr[2][1]);

        det21 = b2[0]*matr[1][1]*matr[2][2]+matr[0][1]*matr[1][2]*b2[2]+matr[0][2]*b2[1]*matr[2][1]
              -(matr[0][2]*matr[1][1]*b2[2]+matr[0][1]*b2[1]*matr[2][2]+b2[0]* matr[1][2]*matr[2][1]);

        det22 = matr[0][0]*b2[1]*matr[2][2]+b2[0]*matr[1][2]*matr[2][0]+matr[0][2]*matr[1][0]*b2[2]
              -(matr[0][2]*b2[1]*matr[2][0]+b2[0]*matr[1][0]*matr[2][2]+matr[0][0]*matr[1][2]*b2[2]);

        det23 = matr[0][0]*matr[1][1]*b2[2]+matr[0][1]*b2[1]*matr[2][0]+b2[0]*matr[1][0]*matr[2][1]
              -(b2[0]*matr[1][1]*matr[2][0]+matr[0][1]*matr[1][0]*b2[2]+matr[0][0]*b2[1]*matr[2][1]);

    double **xx = new double *[3];
    //матрица преобразования
    for(int i = 0;i < 3; i++)
    {
        xx[i] = new double [3];
    }
    xx[0][0] = det11/det1; xx[0][1] = det12/det1; xx[0][2] = det13/det1;
    xx[1][0] = det21/det2; xx[1][1] = det22/det2; xx[1][2] = det23/det2;
    xx[2][0] = 0; xx[2][1] = 0; xx[2][2] = 1;
            //трансформация
    for(int i = 0;i < sWidget->height();i++)
    {
        for(int j = 0;j < sWidget->width();j++)
           {
               double x,y;
               x = xx[0][0]*j+xx[0][1]*i+xx[0][2]*1;
               y = xx[1][0]*j+xx[1][1]*i+xx[1][2]*1;
               if( x >= 0 && y >= 0 && x < image->width() && y <image->height())
               {
                   int xup,xd,yup,yd;
                   xup = ceil(x);
                   xd = floor(x);
                   yup = ceil(y);
                   yd = floor(y);
                   if(xd >0&&yd > 0&&xup < image->width()&&yup < image->height())
                        {
                            double red,blue,green;
                        red = (QColor(image->pixelColor(xd,yd)).red() * (xup - x) + QColor(image->pixelColor(xup,yd)).red() * (x - xd)) *(yup - y) +
     (QColor(image->pixelColor(xd,yup)).red() * (xup - x) +
      QColor(image->pixelColor(xup,yup)).red() * (x - xd)) *(y - yd);

                      blue = (QColor(image->pixelColor(xd,yd)).blue() * (xup - x) + QColor(image->pixelColor(xup,yd)).blue() * (x - xd)) *(yup - y) +
     (QColor(image->pixelColor(xd,yup)).blue() * (xup - x) +
      QColor(image->pixelColor(xup,yup)).blue() * (x - xd)) *(y - yd);

                    green = (QColor(image->pixelColor(xd,yd)).green() * (xup - x) + QColor(image->pixelColor(xup,yd)).green() * (x - xd)) *(yup - y) +
     (QColor(image->pixelColor(xd,yup)).green() * (xup - x) +
      QColor(image->pixelColor(xup,yup)).green() * (x - xd)) *(y - yd);

                            QPen a;
                            a.setColor(QColor(red,green,blue));
                            sWidget->scene->addEllipse(j,i,1,1,a);
                        }
                    }
                 }
                }

}

void MainWindow::paintEvent(QPaintEvent *)
{
    if(oFile != NULL)
    {
        QPixmap map = QPixmap::fromImage(*image);
        map.scaled(fWidget->height(),fWidget->width());
        *image = map.toImage();
        fWidget->scene->addPixmap(map);
    }
    if(fWidget->A!=NULL && fWidget->B!=NULL && fWidget->C!= NULL &&
       sWidget->A!=NULL && sWidget->B!=NULL && sWidget->C!= NULL)
    {
        fWidget->scene->addLine(fWidget->A->GetX(),fWidget->A->GetY(),fWidget->B->GetX(),fWidget->B->GetY(),QPen(Qt::red));
        fWidget->scene->addLine(fWidget->C->GetX(),fWidget->C->GetY(),fWidget->B->GetX(),fWidget->B->GetY(),QPen(Qt::red));
        fWidget->scene->addLine(fWidget->A->GetX(),fWidget->A->GetY(),fWidget->C->GetX(),fWidget->C->GetY(),QPen(Qt::red));
        sWidget->scene->addLine(sWidget->A->GetX(),sWidget->A->GetY(),sWidget->B->GetX(),sWidget->B->GetY(),QPen(Qt::red));
        sWidget->scene->addLine(sWidget->C->GetX(),sWidget->C->GetY(),sWidget->B->GetX(),sWidget->B->GetY(),QPen(Qt::red));
        sWidget->scene->addLine(sWidget->A->GetX(),sWidget->A->GetY(),sWidget->C->GetX(),sWidget->C->GetY(),QPen(Qt::red));
    }
}
