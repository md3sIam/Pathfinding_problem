//
// Created by md3s on 04.02.19.
//

#include "../classes/Graph/Graph.h"
#include "ui_MainWindow.h"
#include "MainWindow.h"
#include <iostream>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    //Setting up the ui
    ui = new Ui::MainWindow;
    ui->setupUi(this);
    setMinimumWidth(1024);
    setMinimumHeight(768);

    //Reading ans setting the graph
    auto g = new Graph;
    g->read_graph_from_files("../maps/nodes/SPb3_nodes.csv",
                             "../maps/roads/SPb3_id_roads.csv");
    std::cout << "Graph is read\n";
    ui->mapWidget->setGraph(g);

    //Setting up actions
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(fileChooser()));
}

MainWindow::~MainWindow() {
    delete ui->menuFile;
    delete ui->menuHelp;
    delete ui->menubar;
    delete ui->mapWidget;
    delete ui->horizontalLayout;
    delete ui->centralwidget;
//    delete ui->tabEditor;
//    delete ui->tabPathfinder;
//    delete ui->tabSettings;
//    delete ui->tabWidget;
    delete ui;
}

QOpenGLWidget* MainWindow::getOpenGLContext() {
    return ui->mapWidget;
}

void MainWindow::fileChooser() {
//    printFileName(QFileDialog::getOpenFileName(this, tr("Open file")));
    QStringList list = QFileDialog::getOpenFileNames(this,tr("Select files"));
    if (list.size() > 1) {
        auto g = new Graph;
        std::cout << list[1].toStdString() << std::endl << list[0].toStdString() << std::endl;
        g->read_graph_from_files(list[1].toStdString(), list[0].toStdString());
        ui->mapWidget->setGraph(g);
    }
}

void MainWindow::printFileName(const QString &str) {
    std::cout << str.toStdString() << std::endl;
}