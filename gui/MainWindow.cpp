//
// Created by md3s on 04.02.19.
//

#include "../classes/Graph/Graph.h"
#include "ui_MainWindow.h"
#include "MainWindow.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include "DefaultGuiSettings.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    //Setting up the ui
    ui = new Ui::MainWindow;
    ui->setupUi(this);
    setMinimumWidth(1024);
    setMinimumHeight(768);

    //Reading ans setting the graph
    graph = new Graph;
    current_filename = dgs::defaultMap;
    graph->read_binary(current_filename);
    std::cout << "Graph is read\n";
    ui->mapWidget->setGraph(graph);
    connect(ui->mapWidget, SIGNAL(enableToSearchPath(bool)), ui->search_button, SLOT(setEnabled(bool)));

    //Setting up actions for toolbar
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(fileNew()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(fileSave()));
    connect(ui->mapWidget, SIGNAL(save()), this, SLOT(fileSave()));
    connect(ui->actionSave_as, SIGNAL(triggered()), this, SLOT(fileSave_as()));

    //Setting up the EDITOR tab
    ui->hl_v_radio->setChecked(false);

    ui->ch_v_color_chooser->setStr("Change color...");
    ui->ch_v_color_chooser->setColor(dgs::vertexColor);
    ui->ch_sel_v_color_chooser->setStr("Change selected color...");
    ui->ch_sel_v_color_chooser->setColor(dgs::selectedVertexColor);

    ui->ch_e_color_chooser->setStr("Change color...");
    ui->ch_e_color_chooser->setColor(dgs::edgeColor);
    ui->ch_sel_e_color_chooser->setStr("Change selected color...");
    ui->ch_sel_e_color_chooser->setColor(dgs::selectedEdgeColor);

    connect(ui->hl_v_radio, SIGNAL(toggled(bool)), ui->mapWidget, SLOT(highlightSl(bool)));
    connect(ui->mapWidget, SIGNAL(highlightSig(bool)), ui->hl_v_radio, SLOT(toggle()));

    //Connecting all buttons with appropriated slots
    connect(ui->link_all_button, SIGNAL(pressed()), ui->mapWidget, SLOT(linkSelVerticesTogether()));
    connect(ui->link_seq_button, SIGNAL(pressed()), ui->mapWidget, SLOT(linkSelVerticesSequentially()));
    connect(ui->rm_sel_v_button, SIGNAL(pressed()), ui->mapWidget, SLOT(removeSelVertices()));
    connect(ui->drop_sel_v_button, SIGNAL(pressed()), ui->mapWidget, SLOT(dropSelVertices()));
    connect(ui->ch_v_color_chooser, SIGNAL(colorChanged(QColor)), ui->mapWidget, SLOT(changeVertexColor(QColor)));
    connect(ui->ch_sel_v_color_chooser, SIGNAL(colorChanged(QColor)), ui->mapWidget, SLOT(changeSelectedVertexColor(QColor)));


    connect(ui->rm_sel_e_button, SIGNAL(pressed()), ui->mapWidget, SLOT(removeSelEdges()));
    connect(ui->drop_sel_e_button, SIGNAL(pressed()), ui->mapWidget, SLOT(dropSelEdges()));
    connect(ui->ch_e_color_chooser, SIGNAL(colorChanged(QColor)), ui->mapWidget, SLOT(changeEdgeColor(QColor)));
    connect(ui->ch_sel_e_color_chooser, SIGNAL(colorChanged(QColor)), ui->mapWidget, SLOT(changeSelectedEdgeColor(QColor)));

    connect(ui->rm_sel_button, SIGNAL(pressed()), ui->mapWidget, SLOT(removeSelEdgesAndVertices()));
    connect(ui->drop_sel_button, SIGNAL(pressed()), ui->mapWidget, SLOT(dropSelEdgesAndVertices()));

    // Setting up the PATHFINDER tab
    ui->bidirectional_checkbox->setChecked(false);
    ui->parallel_checkbox->setEnabled(false);

    connect(ui->bidirectional_checkbox, SIGNAL(toggled(bool)), ui->parallel_checkbox, SLOT(setEnabled(bool)));

    connect(ui->search_button, SIGNAL(clicked()), this, SLOT(findPath()));
    connect(this, SIGNAL(pathFound(const AlgResult*)), ui->mapWidget, SLOT(processResult(const AlgResult*)));
    connect(this, SIGNAL(pathFound(const AlgResult*)), this, SLOT(updateLabelsUponResultGetsFound(const AlgResult*)));

    connect(ui->drop_result_button, SIGNAL(clicked()), ui->mapWidget, SLOT(dropCurrentResultSl()));
    connect(ui->drop_result_button, SIGNAL(clicked()), this, SLOT(dropResultLabels()));

    ui->hlPathCheckBox->setChecked(dgs::hlPath);
    ui->hlFSPCheckBox->setChecked(dgs::hlForwardPath);
    ui->hlRSPCheckBox->setChecked(dgs::hlReversePath);
    ui->hlFSACheckBox->setChecked(dgs::hlForwardEdges);
    ui->hlRSACheckBox->setChecked(dgs::hlReverseEdges);

    connect(ui->hlPathCheckBox, SIGNAL(toggled(bool)), ui->mapWidget, SLOT(hlPathChanged(bool)));
    connect(ui->hlFSPCheckBox, SIGNAL(toggled(bool)), ui->mapWidget, SLOT(hlFSPChanged(bool)));
    connect(ui->hlRSPCheckBox, SIGNAL(toggled(bool)), ui->mapWidget, SLOT(hlRSPChanged(bool)));
    connect(ui->hlFSACheckBox, SIGNAL(toggled(bool)), ui->mapWidget, SLOT(hlFSAChanged(bool)));
    connect(ui->hlRSACheckBox, SIGNAL(toggled(bool)), ui->mapWidget, SLOT(hlRSAChanged(bool)));

    ui->pathColorChooser->setStr("Path Color");
    ui->pathColorChooser->setColor(dgs::pathColor);
    connect(ui->pathColorChooser, SIGNAL(colorChanged(QColor)), ui->mapWidget, SLOT(pathColorChanged(QColor)));

    ui->fspColorChooser->setStr("Forward Path Color");
    ui->fspColorChooser->setColor(dgs::forwardSearchPathEdgeColor);
    connect(ui->fspColorChooser, SIGNAL(colorChanged(QColor)), ui->mapWidget, SLOT(fspColorChanged(QColor)));

    ui->rspColorChooser->setStr("Reverse Path Color");
    ui->rspColorChooser->setColor(dgs::reverseSearchPathEdgeColor);
    connect(ui->rspColorChooser, SIGNAL(colorChanged(QColor)), ui->mapWidget, SLOT(rspColorChanged(QColor)));

    ui->fsaColorChooser->setStr("Forward Search Area Color");
    ui->fsaColorChooser->setColor(dgs::forwardSearchAreaColor);
    connect(ui->fsaColorChooser, SIGNAL(colorChanged(QColor)), ui->mapWidget, SLOT(fsaColorChanged(QColor)));

    ui->rsaColorChooser->setStr("Reverse Search Area Color");
    ui->rsaColorChooser->setColor(dgs::reverseSearchAreaColor);
    connect(ui->rsaColorChooser, SIGNAL(colorChanged(QColor)), ui->mapWidget, SLOT(rsaColorChanged(QColor)));
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

void MainWindow::findPath() {
    Vertex* start, *end;
    start = ui->mapWidget->vertexHandler["Selected"].getSequence()[0];
    end = ui->mapWidget->vertexHandler["Selected"].getSequence()[1];
    bool isBidirectional = ui->bidirectional_checkbox->isChecked();
    bool isParallel = isBidirectional && ui->parallel_checkbox->isChecked();
    int algIndex = ui->algorithm_chooser->currentIndex();
    const AlgResult* res;

    if (!algIndex)
        algIndex = Algorithms::DIJKSTRA;
    else if (algIndex == 1)
        algIndex = Algorithms::DIJKSTRA_WITH_STOP;
    else if (algIndex == 2)
        algIndex = Algorithms::ASTAR;

    res = Algorithms::findPath(algIndex, ui->mapWidget->graph, start, end, isBidirectional, isParallel);
    emit pathFound(res);
}

void MainWindow::updateLabelsUponResultGetsFound(const AlgResult *r) {
    if (r->found) {
        ui->time_result_label->setText({std::to_string(r->time).c_str()});
        std::ostringstream ss;
        ss << r->length;
        ui->length_result_label->setText(ss.str().c_str());
    }
    else {
        dropResultLabels();
    }
}

void MainWindow::dropResultLabels() {
    ui->time_result_label->setText("N/A");
    ui->length_result_label->setText("N/A");
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