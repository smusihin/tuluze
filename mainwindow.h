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
    ~MainWindow() override;

public slots:
    void loadImage();
    void resetImage();
    void splitImage();
    void okUnit();
    void errUnit();
    void missUnit();
    void stopUnit();
    void saveResults();
    void rotate();





private:
    Ui::MainWindow *ui;
    ClipScene * scene_;
    QString image_path_;
    tuluze tuluze_;
    uint32_t current_row_;
    uint32_t current_column_;
    void upUnit();
    void downUnit();
    void leftUnit();
    void rightUnit();
    void change_unit();
    void reload_unit();
    void update_table();
    void keyPressEvent(QKeyEvent *event) override;



};

#endif // MAINWINDOW_H
