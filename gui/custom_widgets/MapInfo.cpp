#include "MapInfo.h"

MapInfo::MapInfo(QWidget *parent) : QFrame(parent) {
    setStyleSheet("MapInfo {background-color: rgba(0,0,0,0.5);"
                  "padding: 2px;}");
//    setMinimumWidth(100);
//    setMinimumHeight(100);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setFrameShape(QFrame::StyledPanel);
    setFrameShadow(QFrame::Plain);
    layout = new QGridLayout(this);
    layout->setColumnMinimumWidth(0, 180);
    QVector<QString> names = {"Vertices",
                              "Edges amount",
                              "Selected vertices",
                              "Selected edges"};
    uint i = 0;
    QString styleSheet = "QLabel {font-size:17px; color: white;}";
    for (const auto &str : names){
        auto label = new QLabel(str, this);
        label->setStyleSheet(styleSheet);
        label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        layout->addWidget(label, i, 0);
        labels.push_back(label);
        label = new QLabel("193207",this);
        label->setStyleSheet(styleSheet);
        label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        layout->addWidget(label, i, 1);
        lcds.push_back(label);
        i++;
    }
}

MapInfo::~MapInfo() {
    for (auto &label : labels){
        delete label;
    }
    for (auto &lcd : lcds){
        delete lcd;
    }
    delete layout;
}

void MapInfo::updateValues(unsigned long va, unsigned long ea,
                           unsigned long sva, unsigned long sea) {
    lcds[0]->setText(QString::number(va));
    lcds[1]->setText(QString::number(ea));
    lcds[2]->setText(QString::number(sva));
    lcds[3]->setText(QString::number(sea));
    repaint();
}