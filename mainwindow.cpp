#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImage()
{
    std::cout << "open file";
}

void MainWindow::cropImage()
{

}

void MainWindow::splitImage()
{

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
