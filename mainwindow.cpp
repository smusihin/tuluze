#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    generalScene = new ClipScene(this);
    ui->generalView->setScene(generalScene);
    connect(generalScene,&ClipScene::clippedRect,this,
            [=](int left,int top,int width, int height)
    {
        tuluze_.crop_image(left,top,width,height);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImage()
{
    currentImagePath = QFileDialog::getOpenFileName(this, "Open Image File", QString(), tr("Image (*.png *.jpg)"));
    ui->fileLbl->setText(currentImagePath.split('/').back());
    tuluze_.load_file(currentImagePath);
    generalScene->setPixmap(tuluze_);
    ui->unitView->setScene(new QGraphicsScene);
    ui->unitView->scene()->addPixmap(tuluze_.get_unit(1,1));
    //ui->generalView->fitInView(generalScene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::cropImage()
{

}

void MainWindow::splitImage()
{
    auto pixmap = tuluze_.split_image();
    generalScene->setPixmap(pixmap);
}

void MainWindow::okUnit()
{

}

void MainWindow::errUnit()
{

}

void MainWindow::stopUnit()
{

}

void MainWindow::saveResults()
{

}
