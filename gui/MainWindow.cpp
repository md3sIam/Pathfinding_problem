//
// Created by md3s on 04.02.19.
//

#include "../classes/Graph/Graph.h"
#include "ui_MainWindow.h"
#include "MainWindow.h"
#include <iostream>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    ui = new Ui::MainWindow;
    ui->setupUi(this);
    auto g = new Graph;
    g->read_graph_from_files("../maps/nodes/SPb3_nodes.csv",
                             "../maps/roads/SPb3_roads.csv");
    std::cout << "Graph is read\n";
    ui->mapWidget->setGraph(g);
    //ui->mapWidget->paintGL();

}

MainWindow::~MainWindow() {
    delete ui;
}

QOpenGLWidget* MainWindow::getOpenGLContext() {
    return ui->mapWidget;
}