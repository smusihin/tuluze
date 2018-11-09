#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clipscene.h"
#include "tuluze.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void loadImage();
    void cropImage();
    void splitImage();
    void okUnit();
    void errUnit();
    void stopUnit();
    void saveResults();




private:
    Ui::MainWindow *ui;
    ClipScene * generalScene;
    QString currentImagePath;
    tuluze tuluze_;

};

#endif // MAINWINDOW_H
