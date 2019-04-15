//
// Created by md3s on 15.04.19.
//

#ifndef PATHFINDING_PROBLEM_ALGTHREAD_H
#define PATHFINDING_PROBLEM_ALGTHREAD_H


#include <QtCore/QThread>
#include <classes/Algorithms.h>

class AlgThread : public QThread {
    Q_OBJECT
private:
    void run() override;

    Graph* g;
    Vertex* startV;
    Vertex* endV;
    bool isB;
    bool isP;
    int algIndex;

signals:
    void algResultFound(const AlgResult*);

public:
    void startThread(int index, Graph*, Vertex*, Vertex*, bool b, bool p);
};


#endif //PATHFINDING_PROBLEM_ALGTHREAD_H
