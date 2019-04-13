//
// Created by md3s on 03.04.19.
//

#ifndef PATHFINDING_PROBLEM_ALGORITHMS_H
#define PATHFINDING_PROBLEM_ALGORITHMS_H

#include <unordered_set>
#include <classes/Graph/Graph.h>
#include <QtCore/QObject>
#include "Category.h"

struct AlgResult {
    bool found = true;
    std::list<Vertex*> path;
    double length = -1;
    double time = -1;

    std::unordered_set<Edge*> forwardSearchPathEdges;
    std::unordered_set<Edge*> reverseSearchPathEdges;
    std::unordered_set<Edge*> forwardSearchEdges;
    std::unordered_set<Edge*> reverseSearchEdges;
};

class Algorithms {

private:
    static const AlgResult* d(const Graph* g, Vertex* start, Vertex* finish);

    static const AlgResult* dws(const Graph* g, Vertex* start, Vertex* finish);
    static const AlgResult* bdws(const Graph* g, Vertex* start, Vertex* finish);
    static const AlgResult* pdws(const Graph* g, Vertex* start, Vertex* finish);

    static const AlgResult* astar(const Graph* g, Vertex* start, Vertex* finish);
    static const AlgResult* bastar(const Graph* g, Vertex* start, Vertex* finish);
    static const AlgResult* pastar(const Graph* g, Vertex* start, Vertex* finish);

public:
    enum Algorithm {DIJKSTRA = 0, DIJKSTRA_WITH_STOP, ASTAR};

    static const AlgResult* findPath(int alg, const Graph* g, Vertex* start, Vertex* end, bool bidirectional = false, bool parallel = false);
};


#endif //PATHFINDING_PROBLEM_ALGORITHMS_H
