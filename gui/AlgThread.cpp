//
// Created by md3s on 15.04.19.
//

#include "AlgThread.h"

void AlgThread::run() {
    emit algResultFound(Algorithms::findPath(algIndex, g, startV, endV, isB, isP));
}

void AlgThread::startThread(int index, Graph* gr, Vertex *s, Vertex *e, bool b, bool p) {
    algIndex = index;
    g = gr;
    startV = s;
    endV = e;
    isB = b;
    isP = p;
    start();
}