//
// Created by md3s on 12.04.19.
//

#ifndef PATHFINDING_PROBLEM_STARTSEARCHBUTTON_H
#define PATHFINDING_PROBLEM_STARTSEARCHBUTTON_H

#include <QtWidgets/QPushButton>
#include <classes/Algorithms.h>

class StartSearchButton : QPushButton {
    Q_OBJECT

public slots:
    void click();

signals:
    void gotResult(const AlgResult* result);

};

void StartSearchButton::click() {

}

#endif //PATHFINDING_PROBLEM_STARTSEARCHBUTTON_H
