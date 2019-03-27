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
    graph = new Graph;
    current_filename = "../maps/binaries/spb3.graph";
    graph->read_binary(current_filename);
    std::cout << "Graph is read\n";
    ui->mapWidget->setGraph(graph);

    //Setting up actions for toolbar
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(fileNew()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(fileSave()));
    connect(ui->mapWidget, SIGNAL(save()), this, SLOT(fileSave()));
    connect(ui->actionSave_as, SIGNAL(triggered()), this, SLOT(fileSave_as()));

    //Setting up the EDITOR tab
    ui->hl_v_radio->setChecked(false);
    connect(ui->hl_v_radio, SIGNAL(toggled(bool)), ui->mapWidget, SLOT(highlightSl(bool)));
    connect(ui->mapWidget, SIGNAL(highlightSig(bool)), ui->hl_v_radio, SLOT(toggle()));

    //Connecting all buttons with appropriated slots
    connect(ui->link_all_button, SIGNAL(pressed()), ui->mapWidget, SLOT(linkSelVerticesTogether()));
    connect(ui->link_seq_button, SIGNAL(pressed()), ui->mapWidget, SLOT(linkSelVerticesSequentially()));
    connect(ui->rm_sel_v_button, SIGNAL(pressed()), ui->mapWidget, SLOT(removeSelVertices()));
    connect(ui->drop_sel_v_button, SIGNAL(pressed()), ui->mapWidget, SLOT(dropSelVertices()));

    connect(ui->rm_sel_e_button, SIGNAL(pressed()), ui->mapWidget, SLOT(removeSelEdges()));
    connect(ui->drop_sel_e_button, SIGNAL(pressed()), ui->mapWidget, SLOT(dropSelEdges()));

    connect(ui->rm_sel_button, SIGNAL(pressed()), ui->mapWidget, SLOT(removeSelEdgesAndVertices()));
    connect(ui->drop_sel_button, SIGNAL(pressed()), ui->mapWidget, SLOT(dropSelEdgesAndVertices()));
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

void MainWindow::fileNew(){
    current_filename = "";
    graph = new Graph;
    ui->mapWidget->setGraph(graph);
}

void MainWindow::fileOpen() {
//    printFileName(QFileDialog::getOpenFileName(this, tr("Open file")));
    QString filename = QFileDialog::getOpenFileName(this,
            tr("Select graph"), "../maps/binaries", tr("Binary files (*.graph)"));
    if (filename.length() > 1) {
        graph = new Graph;
        graph->read_binary(filename.toStdString());
        ui->mapWidget->setGraph(graph);
        current_filename = filename.toStdString();
    }
}

void MainWindow::fileSave() {
    if (current_filename.length() > 1) {
        graph->save_to_binary(current_filename);
    }
    else{
        fileSave_as();
    }
}

void MainWindow::fileSave_as() {
    QString filename = QFileDialog::getSaveFileName(this,
            tr("Choose a file to save"), "../maps/binaries", tr("Binary files (*.graph)"));
    if (filename.length() > 1){
        current_filename = filename.toStdString();
        fileSave();
    }
}

void MainWindow::printFileName(const QString &str) {
    std::cout << str.toStdString() << std::endl;
}