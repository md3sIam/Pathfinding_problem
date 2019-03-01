//
// Created by md3s on 01.03.19.
//

#ifndef PATHFINDING_PROBLEM_MAPINFO_H
#define PATHFINDING_PROBLEM_MAPINFO_H


#include <QFrame>
#include <QLabel>
#include <QLCDNumber>
#include <QGridLayout>

class MapInfo : public QFrame{
    Q_OBJECT
private:
    uint verAmount, edgesAmount, selVerAmount, selEdgesAmount;
    QVector<QLabel*> labels;
    QVector<QLabel*> lcds;
    QGridLayout *layout;

public:
    explicit MapInfo(QWidget* parent = 0);
    ~MapInfo();

public slots:
    void updateValues(unsigned long,unsigned long,unsigned long,unsigned long);

};


#endif //PATHFINDING_PROBLEM_MAPINFO_H
