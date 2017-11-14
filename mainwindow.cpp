#include "mainwindow.h"
#include "ui_mainwindow.h"

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
   oFile = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.bmp *.jpg *.png");
   image1->load(oFile,NULL);

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
    //гаусс
    double  tmp, *xx = new double[10];
    for(int i = 0;i<10;i++)
        xx[i]=0;
        int k,i,j;
        for ( i=0; i<n; i++)
             {
               tmp=matr[i][i];
                 for (j=n;j>=i;j--)
                     matr[i][j]/=tmp;
                   for (j=i+1;j<n;j++)
                  {
                     tmp=matr[j][i];
                       for (k=n;k>=i;k--)
                     matr[j][k]-=tmp*matr[i][k];
                  }
              }
          /*обратный ход*/
            xx[n-1] = matr[n-1][n];
             for (i=n-2; i>=0; i--)
               {
                   xx[i] = matr[i][n];
                   for (j=i+1;j<n;j++) xx[i]-=matr[i][j]*xx[j];
               }
        xx[6]=0;xx[7]=0;xx[8]=1;
        int x,y;
        //трансформация
        for(int i =0;i<image->height();i++)
        {
            for(int j = 0;j<image->width();j++)
            {
                x = xx[0]*i+xx[1]*j+xx[2]*1+1;
                y = xx[3]*i+xx[4]*j+xx[5]*1+1;
                x = floor(x);
                y = floor(y);
                if(x <0)
                    x*=-1;
                if(y<0)
                    y*=-1;
               image1->setPixelColor(x,y,QColor(image->pixel(i,j)));
              // image1->setPixel(x,y,uint(QRgb(image->pixel(i,j))));
            }
        }
        QPixmap map = QPixmap::fromImage(*image1);
        sWidget->scene->addPixmap(map);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    if(oFile != NULL)
    {
        QPixmap map = QPixmap::fromImage(*image);
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
