//
// Created by md3s on 04.02.19.
//

#ifndef PATHFINDING_PROBLEM_MAINWINDOW_H
#define PATHFINDING_PROBLEM_MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QFileDialog>
#include <classes/Algorithms.h>

namespace Ui {
    class MainWindow;
}

class Graph;


class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QOpenGLWidget* getOpenGLContext();

private:
    Ui::MainWindow* ui;
    std::string current_filename;
    Graph* graph;

private slots:
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSave_as();
    void printFileName(const QString& str);

    void findPath();
    void updateLabelsUponResultGetsFound(const AlgResult*);
    void dropResultLabels();

signals:
    void pathFound(const AlgResult*);
};


#endif //PATHFINDING_PROBLEM_MAINWINDOW_H
