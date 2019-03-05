//
// Created by md3s on 04.02.19.
//

#ifndef PATHFINDING_PROBLEM_MAINWINDOW_H
#define PATHFINDING_PROBLEM_MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QFileDialog>

namespace Ui {
    class MainWindow;
}


class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QOpenGLWidget* getOpenGLContext();

private:
    Ui::MainWindow* ui;

private slots:
    void fileChooser();
    void printFileName(const QString& str);


};


#endif //PATHFINDING_PROBLEM_MAINWINDOW_H
