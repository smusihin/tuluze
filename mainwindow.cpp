#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QGraphicsItem>
#include <QKeyEvent>

const QString filename = "tuluze.csv";


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->unitView->setScene(new QGraphicsScene);
    scene_ = new ClipScene(this);
    ui->generalView->setScene(scene_);
    connect(scene_,&ClipScene::clippedRect,this,
            [=](int left,int top,int width, int height)
    {        

        auto items = scene_->items();
        auto rect_pixmap = items.last()->boundingRect();
        auto scale_x = rect_pixmap.width()/tuluze_.width();
        auto scale_y = rect_pixmap.height()/tuluze_.height();
        int32_t l = int32_t((left - rect_pixmap.left()) / scale_x);
        int32_t t = int32_t((top - rect_pixmap.top()) / scale_y);
        int32_t w = int32_t(width/scale_x);
        int32_t h = int32_t(height/scale_y);
        scene_->setPixmap(tuluze_.crop_image(l,t,w,h));        
        ui->generalView->fitInView(scene_->itemsBoundingRect(), Qt::KeepAspectRatio);
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImage()
{
    image_path_ = QFileDialog::getOpenFileName(this, "Open Image File", QString(), tr("Image (*.png *.jpg)"));
    ui->fileLbl->setText(image_path_.split('/').back());
    tuluze_.load_file(image_path_);
    resetImage();
  }

void MainWindow::resetImage()
{
    tuluze_.reset();
    scene_->setPixmap(tuluze_);
    ui->generalView->
            fitInView(scene_->itemsBoundingRect(), Qt::KeepAspectRatio);
    current_column_ = 0;
    current_row_ = 0;
}

void MainWindow::splitImage()
{
    tuluze_.split_image();
    current_column_ = 0;
    current_row_ = 0;
    reload_unit();
}

void MainWindow::okUnit()
{
    tuluze_.set_state(current_row_,current_column_,tuluze::Ok);
    update_table();
    change_unit();
}

void MainWindow::errUnit()
{
    tuluze_.set_state(current_row_,current_column_,tuluze::Error);
    update_table();
    change_unit();
}

void MainWindow::missUnit()
{
    tuluze_.set_state(current_row_,current_column_,tuluze::Not);
    update_table();
    change_unit();
}

void MainWindow::stopUnit()
{
    current_column_ = tuluze_.columns() - 1;
    update_table();
    change_unit();
}

void MainWindow::saveResults()
{
    QFile file(filename);
    if (!file.open(QIODevice::Append | QIODevice::Text))
              return;
    QStringList data;
    data.append(ui->nameEdit->text());
    for (auto i=0; i<4; i++)
    {
        data.append("");
    }
    QStringList errors;
    for(uint32_t i=0; i<tuluze_.rows();i++)
    {
        auto r = tuluze_.row_results(i);
        data.append(QString::number(r.first));
        errors.append(QString::number(r.second));
    }
    data.append(errors);
    file.write((data.join(";")+"\n").toLocal8Bit());
    file.close();
}

void MainWindow::rotate()
{
  //  tuluze_.rotate();
    //  resetImage();
}

void MainWindow::upUnit()
{
   if(current_row_ > 0)
   {
       current_row_--;
       reload_unit();
   }
}

void MainWindow::downUnit()
{
    if(current_row_ < tuluze_.rows()-1)
    {
        current_row_++;
        reload_unit();
    }
}

void MainWindow::leftUnit()
{
    if(current_column_ > 0)
    {
        current_column_--;
        reload_unit();
    }
}

void MainWindow::rightUnit()
{
    if(current_column_ < tuluze_.columns()-1)
    {
        current_column_++;
        reload_unit();
    }
}

void MainWindow::change_unit()
{
    current_column_++;
    if(current_column_ == tuluze_.columns())
    {
        current_column_ = 0;
        current_row_ ++;
        if(current_row_ == tuluze_.rows())
        {
            current_row_ =0;
        }
    }
    reload_unit();
}

void MainWindow::reload_unit()
{
    ui->unitView->scene()->clear();
    ui->unitView->scene()->addPixmap(tuluze_.get_unit(current_row_,current_column_));
    ui->unitView->fitInView(ui->unitView->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);

    scene_->setPixmap(tuluze_.select_unit(current_row_,current_column_));
    ui->generalView->fitInView(scene_->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::update_table()
{
    auto results = tuluze_.row_results(current_row_);
    auto item_total = ui->tableWidget->item(int(current_row_),0);
    if(item_total == nullptr)
    {
        item_total = new QTableWidgetItem;
        ui->tableWidget->setItem(int(current_row_),0,item_total);
    }
    item_total->setText(QString::number(results.first));
    auto item_error = ui->tableWidget->item(int(current_row_),1);
    if(item_error == nullptr)
    {
        item_error = new QTableWidgetItem;
        ui->tableWidget->setItem(int(current_row_),1,item_error);
    }
    item_error->setText(QString::number(results.second));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    auto key = event->key();
    switch (key)
    {
    case Qt::Key_1: okUnit(); break;
    case Qt::Key_2: errUnit(); break;
    case Qt::Key_3: missUnit(); break;
    case Qt::Key_Enter: stopUnit(); break;
    case Qt::Key_A: leftUnit(); break;
    case Qt::Key_D: rightUnit(); break;
    case Qt::Key_W: upUnit();break;
    case Qt::Key_S: downUnit();break;
    default: break;
    }

    QWidget::keyPressEvent(event);
}
