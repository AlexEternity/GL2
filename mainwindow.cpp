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
   //im//age1->load(oFile,NULL);

}

void MainWindow::on_transform_clicked()
{
    fWidget->scene->update();
    sWidget->A->update();
    int n = 6;
    int m = 7;
    double **matr = new double *[n];
    //система
    for(int i = 0;i < 7; i++)
    {
        matr[i] = new double [m];
        if(i == 0 || i == 1 || i == 2)
        {
            if(i == 0)
            {
                matr[i][0] = fWidget->A->GetX();
                matr[i][1] = fWidget->A->GetY();
                matr[i][6] = sWidget->A->GetX();
            }
            else if(i == 1)
            {
                matr[i][0] = fWidget->B->GetX();
                matr[i][1] = fWidget->B->GetY();
                matr[i][6] = sWidget->B->GetX();

            }
            else if(i == 2)
            {
                matr[i][0] = fWidget->C->GetX();
                matr[i][1] = fWidget->C->GetY();
                matr[i][6] = sWidget->C->GetX();
            }
            matr[i][2] = 1;
            for(int j = 3;j < 6;j++)
                matr[i][j] = 0;
        }
        else if(i == 3 || i == 4 || i == 5)
        {
            if(i == 3)
            {
                matr[i][3] = fWidget->A->GetX();
                matr[i][4] = fWidget->A->GetY();
                matr[i][6] = sWidget->A->GetY();
            }
            else if(i == 4)
            {
                matr[i][3] = fWidget->B->GetX();
                matr[i][4] = fWidget->B->GetY();
                matr[i][6] = sWidget->B->GetY();

            }
            else if(i == 5)
            {
                matr[i][3] = fWidget->C->GetX();
                matr[i][4] = fWidget->C->GetY();
                matr[i][6] = sWidget->C->GetY();
            }
            matr[i][5] = 1;
            for(int j = 0;j < 3;j++)
                matr[i][j] = 0;
        }
    }
    //krummer
    double det,det1,det2,det3,det4,det5,det6;
    det = matr[0][0] + matr[1][1] + matr[2][2] + matr[3][3] + matr[4][4] + matr[5][5];
    det1 = matr[0][6] + matr[1][1] + matr[2][2] + matr[3][3] + matr[4][4] + matr[5][5];
    det2 = matr[0][0] + matr[1][6] + matr[2][2] + matr[3][3] + matr[4][4] + matr[5][5];
    det3 = matr[0][0] + matr[1][1] + matr[2][6] + matr[3][3] + matr[4][4] + matr[5][5];
    det4 = matr[0][0] + matr[1][1] + matr[2][2] + matr[3][6] + matr[4][4] + matr[5][5];
    det5 = matr[0][0] + matr[1][1] + matr[2][2] + matr[3][3] + matr[4][6] + matr[5][5];
    det6 = matr[0][0] + matr[1][1] + matr[2][2] + matr[3][3] + matr[4][4] + matr[5][6];
    double **xx = new double *[3];
    //система
    for(int i = 0;i < 3; i++)
    {
        xx[i] = new double [3];
    }
    xx[0][0] = det1/det; xx[0][1] = det2/det; xx[0][2] = det3/det;
    xx[1][0] = det4/det; xx[1][1] = det5/det; xx[1][2] = det6/det;
    xx[2][0] = 0; xx[2][1] = 0; xx[2][2] = 1;
    for(int i =0;i<3;i++)
        for(int j=0;j<3;j++)
            qDebug() << xx[i][j];
    QImage a(sWidget->width(),sWidget->height(),QImage::Format_RGB32);
        //трансформация
        for(int j = 0;j < a.width();j++)
        {
            for(int i = 0;i < a.height();i++)
            {
                double x,y;
                x = xx[0][0]*i+xx[0][1]*j+xx[0][2]*1;
                y = xx[1][0]*i+xx[1][1]*j+xx[1][2]*1;
                if( x >= 0 && y >= 0 && x <= fWidget->width() && y <= fWidget->height())
                {
                    int xup,xd,yup,yd;
                    xup = ceil(x);
                    xd = floor(x);
                    yup = ceil(y);
                    yd = floor(y);
                    if(xd > 0 && yd > 0 && xup < image->width() && yup < image->height())
                    {
                        double red,blue,green;
                        red = (QColor(image->pixel(xd,yd)).red() * (xup - x) + QColor(image->pixel(xup,yd)).red() * (x - xd)) *(yup - y) +
                              (QColor(image->pixel(xd,yup)).red() * (xup - x) + QColor(image->pixel(xup,yup)).red() * (x - xd)) *(y - yd);
                        blue = (QColor(image->pixel(xd,yd)).blue() * (xup - x) + QColor(image->pixel(xup,yd)).blue() * (x - xd)) *(yup - y) +
                                (QColor(image->pixel(xd,yup)).blue() * (xup - x) + QColor(image->pixel(xup,yup)).blue() * (x - xd)) *(y - yd);
                        green = (QColor(image->pixel(xd,yd)).green() * (xup - x) + QColor(image->pixel(xup,yd)).green() * (x - xd)) *(yup - y) +
                                (QColor(image->pixel(xd,yup)).green() * (xup - x) + QColor(image->pixel(xup,yup)).green() * (x - xd)) *(y - yd);
                       a.setPixelColor(i,j,QColor(red,blue,green));
                    }
                }
             }
            }

        QPixmap map = QPixmap::fromImage(a);
        sWidget->scene->addPixmap(map);
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
