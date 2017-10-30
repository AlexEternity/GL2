#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMatrix>
#include <QMatrix4x4>
QTextStream cout(stdout);

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
        Pal.setColor(QPalette::Background, Qt::red);
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
    fWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключим скроллбар по горизонтали
    fWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Отключим скроллбар по вертикали
    fWidget->setAlignment(Qt::AlignCenter);                        // Делаем привязку содержимого к центру
    fWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету
    fWidget->scene = new QGraphicsScene();   // Инициализируем сцену для отрисовки
    fWidget->scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    fWidget->setScene(fWidget->scene);          // Устанавливаем сцену в виджет
    fWidget->setRenderHint(QPainter::Antialiasing);    // Настраиваем рендер
    fWidget->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
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
    sWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключим скроллбар по горизонтали
    sWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Отключим скроллбар по вертикали
    sWidget->setAlignment(Qt::AlignCenter);                        // Делаем привязку содержимого к центру
    sWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету
    sWidget->scene = new QGraphicsScene();   // Инициализируем сцену для отрисовки
    sWidget->scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    sWidget->setScene(sWidget->scene);          // Устанавливаем сцену в виджет
    sWidget->setRenderHint(QPainter::Antialiasing);    // Настраиваем рендер
    sWidget->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    sWidget->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    sWidget->scene->setSceneRect(0,40,sWidget->height(),sWidget->width());
    sWidget->show();
    image = new QImage();
    image1 = new QImage();


    connect( fWidget, SIGNAL(signal1(QString)), qApp, SLOT( on_paint(QString)) );
    //connect( sWidget, SIGNAL(signal1(QString)), qApp, SLOT( on_paint(QString)) );
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
    image1->load(oFile,NULL);
    for(int i =0;i<image1->height();i++)
        for(int j =0;j<image1->width();j++)
            image1->setPixelColor(i,j,QRgb(Qt::white));
}

void MainWindow::on_transform_clicked()
{
    fWidget->scene->update();
    sWidget->A->update();
    int n = 6;
    int m = 7;
    double **matr = new double *[n];
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
            for(int j = 3;j < 7;j++)
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
            for(int i=0;i<6;i++)
            {
                cout<<endl;
                for(int j=0;j<7;j++)
                    cout<<matr[i][j]<< " ";
            }
            cout<<endl;
            for(int i=0;i<9;i++)
                cout<<xx[i]<<" ";
         double sumr = 0,sumb = 0,sumg = 0;
         for(int i = 0;i < image->heightMM();i++)
         {
             QPixmap map1 = QPixmap::fromImage(*image1);
             sWidget->scene->addPixmap(map1);
             for(int j = 0;j < image->widthMM();j++)
             {
                 if( i == 0)
                 {
                    if(j == 0)
                    {

                        sumr = (QColor(image->pixel(i,j)).red())*xx[4] + (QColor(image->pixel(i + 1,j)).red())*xx[7] +
                     (QColor(image->pixel(i,j + 1)).red())*xx[5] + (QColor(image->pixel(i + 1,j + 1)).red())*xx[8];
                        sumr /= 9;
                        sumb = QColor(image->pixel(i,j)).blue()*xx[4] + QColor(image->pixel(i + 1,j)).blue()*xx[7] +
                      QColor(image->pixel(i,j + 1)).blue()*xx[5] + QColor(image->pixel(i + 1,j + 1)).blue()*xx[8];
                        sumb /= 9;
                        sumg = QColor(image->pixel(i,j)).green()*xx[4] + QColor(image->pixel(i + 1,j)).green()*xx[7] +
                      QColor(image->pixel(i,j + 1)).green()*xx[5] + QColor(image->pixel(i + 1,j + 1)).green()*xx[8];
                        sumg /= 9;
                        image1->setPixelColor(i,j,QColor(sumr,sumb,sumg));
                    }
                    else if( j == image->width()-1)
                    {
                        sumr = QColor(image->pixel(i,j)).red()*xx[4] + QColor(image->pixel(i + 1,j)).red()*xx[7] +
                      QColor(image->pixel(i,j - 1)).red()*xx[3] + QColor(image->pixel(i + 1,j -1)).red()*xx[6];
                        sumr /= 9;
                        sumb = QColor(image->pixel(i,j)).blue()*xx[4] + QColor(image->pixel(i + 1,j)).blue()*xx[7] +
                      QColor(image->pixel(i,j - 1)).blue()*xx[3] + QColor(image->pixel(i + 1,j - 1)).blue()*xx[6];
                        sumb /= 9;
                        sumg = QColor(image->pixel(i,j)).green()*xx[4] + QColor(image->pixel(i + 1,j)).green()*xx[7] +
                      QColor(image->pixel(i,j - 1)).green()*xx[3] + QColor(image->pixel(i + 1,j - 1)).green()*xx[6];
                        sumg /= 9;
                        image1->setPixelColor(i,j,QColor(sumr,sumb,sumg));
                    }
                    else
                    {
                        sumr = QColor(image->pixel(i,j)).red()*xx[4] + QColor(image->pixel(i + 1,j)).red()*xx[7] +
                      QColor(image->pixel(i,j - 1)).red()*xx[3] + QColor(image->pixel(i + 1,j -1)).red()*xx[6] +
                      QColor(image->pixel(i,j + 1)).red()*xx[5] + QColor(image->pixel(i + 1,j + 1)).red()*xx[8];
                        sumr /= 9;
                        sumb = QColor(image->pixel(i,j)).blue()*xx[4] + QColor(image->pixel(i + 1,j)).blue()*xx[7] +
                      QColor(image->pixel(i,j - 1)).blue()*xx[3] + QColor(image->pixel(i + 1,j - 1)).blue()*xx[6] +
                      QColor(image->pixel(i,j + 1)).blue()*xx[5] + QColor(image->pixel(i + 1,j + 1)).blue()*xx[8];
                        sumb /= 9;
                        sumg = QColor(image->pixel(i,j)).green()*xx[4] + QColor(image->pixel(i + 1,j)).green()*xx[7] +
                      QColor(image->pixel(i,j - 1)).green()*xx[3] + QColor(image->pixel(i + 1,j - 1)).green()*xx[6] +
                      QColor(image->pixel(i,j + 1)).green()*xx[5] + QColor(image->pixel(i + 1,j + 1)).green()*xx[8];
                        sumg /= 9;
                        image1->setPixelColor(i,j,QColor(sumr,sumb,sumg));
                    }
                 }
                 else if( j == 0)
                 {
                    if(i == image->height() - 1)
                    {
                        sumr = QColor(image->pixel(i,j)).red()*xx[4] + QColor(image->pixel(i - 1,j)).red()*xx[1] +
                      QColor(image->pixel(i - 1,j + 1)).red()*xx[2] + QColor(image->pixel(i,j + 1)).red()*xx[5];
                        sumr /= 9;
                        sumb = QColor(image->pixel(i,j)).blue()*xx[4] + QColor(image->pixel(i - 1,j)).blue()*xx[1] +
                      QColor(image->pixel(i - 1,j + 1)).blue()*xx[2] + QColor(image->pixel(i,j + 1)).blue()*xx[5];
                        sumb /= 9;
                        sumg = QColor(image->pixel(i,j)).green()*xx[4] + QColor(image->pixel(i - 1,j)).green()*xx[1] +
                      QColor(image->pixel(i - 1,j + 1)).green()*xx[2] + QColor(image->pixel(i,j + 1)).green()*xx[5];
                        sumg /= 9;
                        image1->setPixelColor(i,j,QColor(sumr,sumb,sumg));
                    }
                    else
                    {
                        sumr = QColor(image->pixel(i,j)).red()*xx[4] + QColor(image->pixel(i + 1,j)).red()*xx[7] +
                      QColor(image->pixel(i - 1,j)).red()*xx[1] + QColor(image->pixel(i + 1,j + 1)).red()*xx[8] +
                      QColor(image->pixel(i - 1,j + 1)).red()*xx[2] + QColor(image->pixel(i,j + 1)).red()*xx[5];                              ;
                        sumr /= 9;
                        sumb = QColor(image->pixel(i,j)).blue()*xx[4] + QColor(image->pixel(i + 1,j)).blue()*xx[7] +
                      QColor(image->pixel(i - 1,j)).blue()*xx[1] + QColor(image->pixel(i + 1,j + 1)).blue()*xx[8] +
                      QColor(image->pixel(i - 1,j + 1)).blue()*xx[2] + QColor(image->pixel(i,j + 1)).blue()*xx[5];
                        sumb /= 9;
                        sumg = QColor(image->pixel(i,j)).green()*xx[4] + QColor(image->pixel(i + 1,j)).green()*xx[7] +
                      QColor(image->pixel(i - 1,j)).green()*xx[1] + QColor(image->pixel(i + 1,j + 1)).green()*xx[8] +
                      QColor(image->pixel(i - 1,j + 1)).green()*xx[2] + QColor(image->pixel(i,j + 1)).green()*xx[5];
                        sumg /= 9;
                        image1->setPixelColor(i,j,QColor(sumr,sumb,sumg));
                    }
                 }
                 else if( i == (image->height() - 1))
                 {
                    if( j == image->width() - 1)
                    {

                        sumr = QColor(image->pixel(i,j)).red()*xx[4] + QColor(image->pixel(i - 1,j)).red()*xx[1] +
                      QColor(image->pixel(i - 1,j - 1)).red()*xx[0] + QColor(image->pixel(i,j - 1)).red()*xx[3];                              ;
                        sumr /= 9;
                        sumb = QColor(image->pixel(i,j)).blue()*xx[4] + QColor(image->pixel(i - 1,j)).blue()*xx[1] +
                      QColor(image->pixel(i - 1,j - 1)).blue()*xx[0] + QColor(image->pixel(i,j - 1)).blue()*xx[3];
                        sumb /= 9;
                        sumg = QColor(image->pixel(i,j)).green()*xx[4] + QColor(image->pixel(i - 1,j)).green()*xx[1] +
                      QColor(image->pixel(i - 1,j - 1)).green()*xx[0] + QColor(image->pixel(i,j - 1)).green()*xx[3];
                        sumg /= 9;
                        image1->setPixelColor(i,j,QColor(sumr,sumb,sumg));
                    }
                    else
                    {
                        sumr = QColor(image->pixel(i,j)).red()*xx[4] + QColor(image->pixel(i,j - 1)).red()*xx[3] +
                      QColor(image->pixel(i - 1,j - 1)).red()*xx[0] + QColor(image->pixel(i - 1,j)).red()*xx[1] +
                      QColor(image->pixel(i - 1,j + 1)).red()*xx[2] + QColor(image->pixel(i,j + 1)).red()*xx[5];                              ;
                        sumr /= 9;
                        sumb = QColor(image->pixel(i,j)).blue()*xx[4] + QColor(image->pixel(i,j - 1)).blue()*xx[3] +
                      QColor(image->pixel(i - 1,j - 1)).blue()*xx[0] + QColor(image->pixel(i - 1,j)).blue()*xx[1] +
                      QColor(image->pixel(i - 1,j + 1)).blue()*xx[2] + QColor(image->pixel(i,j + 1)).blue()*xx[5];
                        sumb /= 9;
                        sumg = QColor(image->pixel(i,j)).green()*xx[4] + QColor(image->pixel(i,j - 1)).green()*xx[3] +
                      QColor(image->pixel(i - 1,j - 1)).green()*xx[0] + QColor(image->pixel(i -1,j)).green()*xx[1] +
                      QColor(image->pixel(i - 1,j + 1)).green()*xx[2] + QColor(image->pixel(i,j + 1)).green()*xx[5];
                        sumg /= 9;
                        image1->setPixelColor(i,j,QColor(sumr,sumb,sumg));
                    }
                 }
                 else if( j == (image->width()-1))
                 {
                     sumr = QColor(image->pixel(i,j)).red()*xx[4] + QColor(image->pixel(i - 1,j)).red()*xx[1] +
                     QColor(image->pixel(i - 1,j - 1)).red()*xx[0] + QColor(image->pixel(i,j - 1)).red()*xx[3] +
                     QColor(image->pixel(i + 1,j - 1)).red()*xx[6] + QColor(image->pixel(i + 1,j)).red()*xx[7];                              ;
                     sumr /= 9;
                     sumb = QColor(image->pixel(i,j)).blue()*xx[4] + QColor(image->pixel(i + 1,j - 1)).blue()*xx[1] +
                     QColor(image->pixel(i - 1,j - 1)).blue()*xx[0] + QColor(image->pixel(i - 1,j)).blue()*xx[3] +
                     QColor(image->pixel(i,j - 1)).blue()*xx[6] + QColor(image->pixel(i,j + 1)).blue()*xx[7];
                     sumb /= 9;
                     sumg = QColor(image->pixel(i,j)).green()*xx[4] + QColor(image->pixel(i + 1,j - 1)).green()*xx[1] +
                     QColor(image->pixel(i - 1,j - 1)).green()*xx[0] + QColor(image->pixel(i -1,j)).green()*xx[3] +
                     QColor(image->pixel(i,j - 1)).green()*xx[6] + QColor(image->pixel(i,j + 1)).green()*xx[7];
                     sumg /= 9;
                     image1->setPixelColor(i,j,QColor(sumr,sumb,sumg));
                 }
                 else
                 {
                     sumr = QColor(image->pixel(i,j)).red()*xx[4] + QColor(image->pixel(i - 1,j - 1)).red()*xx[0] +
                             QColor(image->pixel(i - 1,j)).red()*xx[1] + QColor(image->pixel(i - 1,j + 1)).red()*xx[2] +
                             QColor(image->pixel(i,j + 1)).red()*xx[5] + QColor(image->pixel(i + 1,j + 1)).red()*xx[8] +
                             QColor(image->pixel(i + 1,j)).red()*xx[7] + QColor(image->pixel(i + 1,j - 1)).red()*xx[6] +
                             QColor(image->pixel(i,j - 1)).red()*xx[3];
                     sumr /= 9;
                     sumb = QColor(image->pixel(i,j)).blue()*xx[4] + QColor(image->pixel(i - 1,j - 1)).blue()*xx[0] +
                             QColor(image->pixel(i - 1,j)).blue()*xx[1] + QColor(image->pixel(i - 1,j + 1)).blue()*xx[2] +
                             QColor(image->pixel(i,j + 1)).blue()*xx[5] + QColor(image->pixel(i + 1,j + 1)).blue()*xx[8] +
                             QColor(image->pixel(i + 1,j)).blue()*xx[7] + QColor(image->pixel(i + 1,j - 1)).blue()*xx[6] +
                             QColor(image->pixel(i,j - 1)).blue()*xx[3];
                     sumb /= 9;
                     sumg = QColor(image->pixel(i,j)).green()*xx[4] + QColor(image->pixel(i - 1,j - 1)).green()*xx[0] +
                             QColor(image->pixel(i - 1,j)).green()*xx[1] + QColor(image->pixel(i - 1,j + 1)).green()*xx[2] +
                             QColor(image->pixel(i,j + 1)).green()*xx[5] + QColor(image->pixel(i + 1,j + 1)).green()*xx[8] +
                             QColor(image->pixel(i + 1,j)).green()*xx[7] + QColor(image->pixel(i + 1,j - 1)).green()*xx[6] +
                             QColor(image->pixel(i,j - 1)).green()*xx[3];
                     sumg /= 9;
                     image1->setPixelColor(i,j,QColor(sumr,sumb,sumg));
                 }
             }
}

}

void MainWindow::paintEvent(QPaintEvent *)
{
    if(oFile != NULL)
    {
        QPixmap map = QPixmap::fromImage(*image);
       // map = map.scaled(fWidget->width(),fWidget->height());
        //*image = map.toImage();
        *image1 = map.toImage();
        fWidget->scene->addPixmap(map);
    }
}
