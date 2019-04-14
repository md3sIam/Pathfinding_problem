//
// Created by md3s on 03.04.19.
//

#include <queue>
#include <classes/Edge/Edge.h>
#include "Algorithms.h"
#include <ctime>
#include <cmath>
#include <iostream>

const AlgResult* Algorithms::findPath(int alg, const Graph* graph, Vertex* start, Vertex* end, bool bidirectional, bool parallel) {
    switch (alg){

        case DIJKSTRA:
            return d(graph, start, end);
            break;

        case DIJKSTRA_WITH_STOP:
            if (bidirectional)
                if (parallel) return pdws(graph, start, end);
                else return bdws(graph, start, end);
            else return dws(graph, start, end);
            break;

        case ASTAR:
            if (bidirectional)
                if (parallel) return pastar(graph, start, end);
                else return bastar(graph, start, end);
            else return astar(graph, start, end);
            break;

        default:
            break;
    }
}

const AlgResult* Algorithms::d(const Graph *g, Vertex *start, Vertex *finish) {
    // initialization of result variable
    auto result = new AlgResult;
    result->path.push_front(start);

    // initialization of working maps
    std::unordered_set<const Vertex*> is_processed;
    std::unordered_map<const Vertex*, double> dist;
    dist.insert({start, 0.0});
    std::unordered_map<const Vertex*, Edge*> edge_to;

    // priority queue initialization
    auto comparator = [&dist](const Vertex* l, const Vertex* r){
        return dist.find(l)->second > dist.find(r)->second;
    };
    std::priority_queue<const Vertex*, std::vector<const Vertex*>, decltype(comparator)> queue(comparator);

    // official algorithm start
    time_t time_start = std::clock();
    queue.push(start);
    // while queue is not empty
    while (!queue.empty()){
        // picking the closest vertex
        //std::cout << "Main Cycle: " << ++c << std::endl;
        const Vertex* currentVertex = queue.top();
        queue.pop();

        // the vertex becomes processed
        is_processed.insert(currentVertex);

        // every non-processed incident vertex is going to the queue
        for (Edge* edge : currentVertex->incidentEdges){
            result->forwardSearchEdges.insert(edge);
            // getting next incident vertex
            const Vertex* nextVertex = currentVertex == edge->vFrom ? edge->vTo : edge->vFrom;
            // if it's processed, skip it
            if (is_processed.find(nextVertex) != is_processed.end()) continue;
            // getting new dist
            double currentDist = dist[currentVertex] + edge->weight;
             // if it's not computed or less than already computed, change dist
             // if for the first time
            if (dist.find(nextVertex) == dist.end()){
                dist.insert({nextVertex, currentDist});
                edge_to.insert({nextVertex, edge});
                queue.push(nextVertex);
            }
            else if (dist[nextVertex] > currentDist){
                dist[nextVertex] = currentDist;
                edge_to[nextVertex] = edge;
            }
        }
    }

    // algorithm done, checking for path existence
    if (dist.find(finish) == dist.end()){
        result->time = double(clock() - time_start) / CLOCKS_PER_SEC;
        result->found = false;
        return result;
    }
    // building the path
    result->length = dist[finish];
    result->path.push_front(finish);
    Vertex* currentVertex = finish;
    double length_check = 0;
    do {
        //std::cout << "Building path: " << ++c << std::endl;
        Edge* currentEdge = edge_to[currentVertex];
        //std::cout << currentEdge->weight << std::endl;
        length_check += currentEdge->weight;
        result->forwardSearchPathEdges.insert(currentEdge);
        currentVertex = currentVertex == currentEdge->vFrom ? currentEdge->vTo : currentEdge->vFrom;
        result->path.push_front(currentVertex);
        //std::cout << "Current Vertex Id: " << currentVertex->id << std::endl;
    } while (currentVertex != start);
    std::cout << "Real length: " << length_check << std::endl;
    result->time = double(clock() - time_start) / CLOCKS_PER_SEC;
    return result;
}

const AlgResult* Algorithms::dws(const Graph* g, Vertex* start, Vertex* finish) {
    // initialization of result variable
    auto result = new AlgResult;
    result->path.push_front(start);

    // initialization of working maps
    std::unordered_set<const Vertex*> is_processed;
    std::unordered_map<const Vertex*, double> dist;
    dist.insert({start, 0.0});
    std::unordered_map<const Vertex*, Edge*> edge_to;

    // priority queue initialization
    auto comparator = [&dist](const Vertex* l, const Vertex* r){
        return dist.find(l)->second > dist.find(r)->second;
    };
    std::priority_queue<const Vertex*, std::vector<const Vertex*>, decltype(comparator)> queue(comparator);

    // official algorithm start
    time_t time_start = std::clock();
    queue.push(start);
    // while queue is not empty
    while (!queue.empty()){
        // picking the closest vertex
        //std::cout << "Main Cycle: " << ++c << std::endl;
        const Vertex* currentVertex = queue.top();
        queue.pop();

        // break if finish vertex
        if (currentVertex == finish) break;

        // the vertex becomes processed
        is_processed.insert(currentVertex);

        // every non-processed incident vertex is going to the queue
        for (Edge* edge : currentVertex->incidentEdges){
            result->forwardSearchEdges.insert(edge);
            // getting next incident vertex
            const Vertex* nextVertex = currentVertex == edge->vFrom ? edge->vTo : edge->vFrom;
            // if it's processed, skip it
            if (is_processed.find(nextVertex) != is_processed.end()) continue;
            // getting new dist
            double currentDist = dist[currentVertex] + edge->weight;
            // if it's not computed or less than already computed, change dist
            // if for the first time
            if (dist.find(nextVertex) == dist.end()){
                dist.insert({nextVertex, currentDist});
                edge_to.insert({nextVertex, edge});
                queue.push(nextVertex);
            }
            else if (dist[nextVertex] > currentDist){
                dist[nextVertex] = currentDist;
                edge_to[nextVertex] = edge;
            }
        }
    }

    // algorithm done, checking for path existence
    if (dist.find(finish) == dist.end()){
        result->time = double(clock() - time_start) / CLOCKS_PER_SEC;
        result->found = false;
        return result;
    }
    // building the path
    result->length = dist[finish];
    result->path.push_front(finish);
    Vertex* currentVertex = finish;
    double length_check = 0;
    do {
        //std::cout << "Building path: " << ++c << std::endl;
        Edge* currentEdge = edge_to[currentVertex];
        //std::cout << currentEdge->weight << std::endl;
        length_check += currentEdge->weight;
        result->forwardSearchPathEdges.insert(currentEdge);
        currentVertex = currentVertex == currentEdge->vFrom ? currentEdge->vTo : currentEdge->vFrom;
        result->path.push_front(currentVertex);
        //std::cout << "Current Vertex Id: " << currentVertex->id << std::endl;
    } while (currentVertex != start);
    std::cout << "Real length: " << length_check << std::endl;
    result->time = double(clock() - time_start) / CLOCKS_PER_SEC;
    return result;
}

const AlgResult* Algorithms::bdws(const Graph* g, Vertex* start, Vertex* finish) {}

const AlgResult* Algorithms::pdws(const Graph* g, Vertex* start, Vertex* finish) {}

const AlgResult* Algorithms::astar(const Graph* g, Vertex* start, Vertex* finish) {
    // initialization of result variable
    auto result = new AlgResult;
    result->path.push_front(start);

    // initialization of working maps
    std::unordered_set<const Vertex*> is_processed;
    std::unordered_map<const Vertex*, double> dist;
    dist.insert({start, 0.0});
    std::unordered_map<const Vertex*, double> minDistToFinish;
    minDistToFinish.insert({start, 0.0});
    std::unordered_map<const Vertex*, Edge*> edge_to;

    // priority queue initialization
    auto comparator = [&dist, &minDistToFinish](const Vertex* l, const Vertex* r){
        return dist.find(l)->second + minDistToFinish.find(l)->second > dist.find(r)->second + minDistToFinish.find(r)->second;
    };
    std::priority_queue<const Vertex*, std::vector<const Vertex*>, decltype(comparator)> queue(comparator);

    // official algorithm start
    time_t time_start = std::clock();
    queue.push(start);
    // while queue is not empty
    while (!queue.empty()){
        // picking the closest vertex
        //std::cout << "Main Cycle: " << ++c << std::endl;
        const Vertex* currentVertex = queue.top();
        queue.pop();

        // break if finish vertex
        if (currentVertex == finish) break;

        // the vertex becomes processed
        is_processed.insert(currentVertex);

        // every non-processed incident vertex is going to the queue
        for (Edge* edge : currentVertex->incidentEdges){
            result->forwardSearchEdges.insert(edge);
            // getting next incident vertex
            const Vertex* nextVertex = currentVertex == edge->vFrom ? edge->vTo : edge->vFrom;
            // if it's processed, skip it
            if (is_processed.find(nextVertex) != is_processed.end()) continue;
            // getting new dist
            double currentDist = dist[currentVertex] + edge->weight;
            // if it's not computed or less than already computed, change dist
            // if for the first time
            if (dist.find(nextVertex) == dist.end()){
                dist.insert({nextVertex, currentDist});
                edge_to.insert({nextVertex, edge});
                minDistToFinish.insert({nextVertex, evklidDistance(nextVertex, finish)});
                queue.push(nextVertex);
            }
            else if (dist[nextVertex] > currentDist){
                dist[nextVertex] = currentDist;
                edge_to[nextVertex] = edge;
            }
        }
    }

    // algorithm done, checking for path existence
    if (dist.find(finish) == dist.end()){
        result->time = double(clock() - time_start) / CLOCKS_PER_SEC;
        result->found = false;
        return result;
    }
    // building the path
    result->length = dist[finish];
    result->path.push_front(finish);
    Vertex* currentVertex = finish;
    double length_check = 0;
    do {
        //std::cout << "Building path: " << ++c << std::endl;
        Edge* currentEdge = edge_to[currentVertex];
        //std::cout << currentEdge->weight << std::endl;
        length_check += currentEdge->weight;
        result->forwardSearchPathEdges.insert(currentEdge);
        currentVertex = currentVertex == currentEdge->vFrom ? currentEdge->vTo : currentEdge->vFrom;
        result->path.push_front(currentVertex);
        //std::cout << "Current Vertex Id: " << currentVertex->id << std::endl;
    } while (currentVertex != start);
    std::cout << "Real length: " << length_check << std::endl;
    result->time = double(clock() - time_start) / CLOCKS_PER_SEC;
    return result;
}

const AlgResult*Algorithms::bastar(const Graph* g, Vertex* start, Vertex* finish) {}

const AlgResult* Algorithms::pastar(const Graph* g, Vertex* start, Vertex* finish) {}

// attended
double Algorithms::evklidDistance(const Vertex *a, const Vertex *b) {
    double sq_d_lon = pow(a->lon - b->lon, 2);
    double sq_d_lat = pow(a->lat - b->lat, 2);
    return sqrt(sq_d_lat + sq_d_lon);
}