//
// Created by md3s on 03.04.19.
//

#include <queue>
#include <classes/Edge/Edge.h>
#include "Algorithms.h"
#include <ctime>
#include <cmath>
#include <iostream>
#include <thread>
#include <mutex>

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

const AlgResult* Algorithms::bdws(const Graph* g, Vertex* start, Vertex* finish) {
    const int AMOUNT_OF_COMMON = 10;
    //// initialization of result variable
    auto result = new AlgResult;

    //// initialization of working maps
    // forward search
    std::unordered_set<const Vertex*> is_processed;
    std::unordered_map<const Vertex*, double> dist;
    dist.insert({start, 0.0});
    std::unordered_map<const Vertex*, Edge*> edge_to;
    // reverse search
    std::unordered_set<const Vertex*> is_processed_r;
    std::unordered_map<const Vertex*, double> dist_r;
    dist_r.insert({finish, 0.0});
    std::unordered_map<const Vertex*, Edge*> edge_to_r;

    //// priority queues initialization
    auto comparator = [&dist](const Vertex* l, const Vertex* r){
        return dist.find(l)->second > dist.find(r)->second;
    };
    auto comparator_r = [&dist_r](const Vertex* l, const Vertex* r){
        return dist_r.find(l)->second > dist_r.find(r)->second;
    };
    std::priority_queue<Vertex*, std::vector<Vertex*>, decltype(comparator)> queue(comparator);
    std::priority_queue<Vertex*, std::vector<Vertex*>, decltype(comparator_r)> queue_r(comparator_r);


    //// amount of meetings of 2 searches
    int meetings = 0;
    bool forward_end = false,
         reverse_end = false;
    Vertex* center = nullptr;

    //// official algorithm start
    time_t time_start = std::clock();
    queue.push(start);
    queue_r.push(finish);
    // while both queues are not empty and amount of meetings is less then 20
    //int c = 0;
    while (!(queue.empty() || queue_r.empty()) && meetings < AMOUNT_OF_COMMON ){
        //std::cout << ++c << std::endl;
        //// forward step
        // picking the closest vertex
        //std::cout << "Main Cycle: " << ++c << std::endl;
        Vertex* currentVertex = queue.top();
        queue.pop();

        // break if finish vertex
        if (currentVertex == finish) {forward_end = true; break;}

        // the vertex becomes processed
        is_processed.insert(currentVertex);

        if (is_processed_r.find(currentVertex) != is_processed_r.end()){
            if (center == nullptr || dist[currentVertex] + dist_r[currentVertex] < dist[center] + dist_r[center]){
                center = currentVertex;
                meetings = 0;
                //std::cout << "new f center\n";
            }
            else if (++meetings > AMOUNT_OF_COMMON ) break;
        }

        // every non-processed incident vertex is going to the queue
        for (Edge* edge : currentVertex->incidentEdges){
            result->forwardSearchEdges.insert(edge);
            // getting next incident vertex
            Vertex* nextVertex = currentVertex == edge->vFrom ? edge->vTo : edge->vFrom;
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

        //// reverse step
        // picking the closest vertex
        //std::cout << "Main Cycle: " << ++c << std::endl;
        currentVertex = queue_r.top();
        queue_r.pop();

        // break if finish vertex
        if (currentVertex == start) {reverse_end = true; break;}

        // the vertex becomes processed
        is_processed_r.insert(currentVertex);

        // checking for the meeting
        if (is_processed.find(currentVertex) != is_processed.end()){
            if (center == nullptr || dist[currentVertex] + dist_r[currentVertex] < dist[center] + dist_r[center]){
                center = currentVertex;
                meetings = 0;
                //std::cout << "new r center\n";
            }
            else if (++meetings > AMOUNT_OF_COMMON) break;
        }

        // every non-processed incident vertex is going to the queue
        for (Edge* edge : currentVertex->incidentEdges){
            result->reverseSearchEdges.insert(edge);
            // getting next incident vertex
            Vertex* nextVertex = currentVertex == edge->vFrom ? edge->vTo : edge->vFrom;
            // if it's processed, skip it
            if (is_processed_r.find(nextVertex) != is_processed_r.end()) continue;
            // getting new dist
            double currentDist = dist_r[currentVertex] + edge->weight;
            // if it's not computed or less than already computed, change dist
            // if for the first time
            if (dist_r.find(nextVertex) == dist_r.end()){
                dist_r.insert({nextVertex, currentDist});
                edge_to_r.insert({nextVertex, edge});
                queue_r.push(nextVertex);
            }
            else if (dist_r[nextVertex] > currentDist){
                dist_r[nextVertex] = currentDist;
                edge_to_r[nextVertex] = edge;
            }
        }
    }

    //// algorithm done, checking for path existence
    if (center == nullptr){
        result->time = double(clock() - time_start) / CLOCKS_PER_SEC;
        result->found = false;
        return result;
    }
    //// building the path
    // forward search part
    Vertex* currentVertex = center;
    result->path.push_back(center);
    do {
        //std::cout << "Building path: " << ++c << std::endl;
        Edge* currentEdge = edge_to[currentVertex];
        result->length += currentEdge->weight;
        //std::cout << currentEdge->weight << std::endl;
        //length_check += currentEdge->weight;
        result->forwardSearchPathEdges.insert(currentEdge);
        currentVertex = currentVertex == currentEdge->vFrom ? currentEdge->vTo : currentEdge->vFrom;
        result->path.push_front(currentVertex);
        //std::cout << "Current Vertex Id: " << currentVertex->id << std::endl;
    } while (currentVertex != start);

    // reverse search part
    currentVertex = center;
    do {
        //std::cout << "Building path: " << ++c << std::endl;
        Edge* currentEdge = edge_to_r[currentVertex];
        result->length += currentEdge->weight;
        //std::cout << currentEdge->weight << std::endl;
        //length_check += currentEdge->weight;
        result->reverseSearchPathEdges.insert(currentEdge);
        currentVertex = currentVertex == currentEdge->vFrom ? currentEdge->vTo : currentEdge->vFrom;
        result->path.push_back(currentVertex);
        //std::cout << "Current Vertex Id: " << currentVertex->id << std::endl;
    } while (currentVertex != finish);
    result->time = double(clock() - time_start) / CLOCKS_PER_SEC;
    for (Edge* edge : result->forwardSearchEdges)
        if (result->reverseSearchEdges.find(edge) != result->reverseSearchEdges.end())
            result->commonSearchEdges.insert(edge);
    return result;
}


const AlgResult* Algorithms::pdws(const Graph *g, Vertex *start, Vertex *finish) {
    const int AMOUNT_OF_COMMON = 10;
    //// initialization of result variable
    auto result = new AlgResult;

    //// initialization of working maps
    // forward search
    std::unordered_set<const Vertex*> is_processed;
    std::unordered_map<const Vertex*, double> dist;
    dist.insert({start, 0.0});
    std::unordered_map<const Vertex*, Edge*> edge_to;
    // reverse search
    std::unordered_set<const Vertex*> is_processed_r;
    std::unordered_map<const Vertex*, double> dist_r;
    dist_r.insert({finish, 0.0});
    std::unordered_map<const Vertex*, Edge*> edge_to_r;

    //// priority queues initialization
    auto comparator = [&dist](const Vertex* l, const Vertex* r){
        return dist.find(l)->second > dist.find(r)->second;
    };
    auto comparator_r = [&dist_r](const Vertex* l, const Vertex* r){
        return dist_r.find(l)->second > dist_r.find(r)->second;
    };
    std::priority_queue<Vertex*, std::vector<Vertex*>, decltype(comparator)> queue(comparator);
    std::priority_queue<Vertex*, std::vector<Vertex*>, decltype(comparator_r)> queue_r(comparator_r);


    //// amount of meetings of 2 searches
    int meetings = 0;
    bool forward_end = false,
            reverse_end = false;
    Vertex* center = nullptr;

    //// preparing thread for forward search
    std::mutex centerMutex;
    std::mutex meetingsMutex;
    std::mutex is_processed_rMutex;
    std::mutex is_processedMutex;
    std::mutex dist_rMutex;
    std::mutex distMutex;

    auto reverse_lambda = [&start, &finish, &is_processed, &dist, &edge_to_r, &queue_r, &result, &is_processed_r, &center, &meetings, &dist_r,
            &centerMutex, &meetingsMutex, &is_processed_rMutex, &is_processedMutex, &dist_rMutex, &distMutex](){
        queue_r.push(finish);
        while (!queue_r.empty()){
            Vertex* currentVertex = queue_r.top();
            queue_r.pop();

            // break if finish vertex
            if (currentVertex == start) {/*reverse_end = true;*/ break;}

            // the vertex becomes processed
            is_processed_rMutex.lock();
            is_processed_r.insert(currentVertex);
            is_processed_rMutex.unlock();

            // checking for the meeting
            is_processedMutex.lock();
            if (is_processed.find(currentVertex) != is_processed.end()){
                centerMutex.lock();
                meetingsMutex.lock();
                distMutex.lock();
                if (center == nullptr || dist[currentVertex] + dist_r[currentVertex] < dist[center] + dist_r[center]){
                    center = currentVertex;
                    meetings = 0;
                    //std::cout << "new r center\n";
                }
                else if (++meetings > AMOUNT_OF_COMMON) {
                    centerMutex.unlock();
                    meetingsMutex.unlock();
                    distMutex.unlock();
                    is_processedMutex.unlock();
                    break;
                }
                centerMutex.unlock();
                meetingsMutex.unlock();
                distMutex.unlock();
            }
            is_processedMutex.unlock();

            // every non-processed incident vertex is going to the queue
            for (Edge* edge : currentVertex->incidentEdges){
                result->reverseSearchEdges.insert(edge);
                // getting next incident vertex
                Vertex* nextVertex = currentVertex == edge->vFrom ? edge->vTo : edge->vFrom;
                // if it's processed, skip it
                if (is_processed_r.find(nextVertex) != is_processed_r.end()) continue;
                // getting new dist
                double currentDist = dist_r[currentVertex] + edge->weight;
                // if it's not computed or less than already computed, change dist
                // if for the first time
                if (dist_r.find(nextVertex) == dist_r.end()){
                    dist_rMutex.lock();
                    dist_r.insert({nextVertex, currentDist});
                    dist_rMutex.unlock();
                    edge_to_r.insert({nextVertex, edge});
                    queue_r.push(nextVertex);
                }
                else if (dist_r[nextVertex] > currentDist){
                    dist_rMutex.lock();
                    dist_r[nextVertex] = currentDist;
                    dist_rMutex.unlock();
                    edge_to_r[nextVertex] = edge;
                }
            }
        }
    };


    //// official algorithm start
    std::thread reverse_thread(reverse_lambda);
    queue.push(start);
    time_t time_start = clock();
    while (!queue.empty()){
        Vertex* currentVertex = queue.top();
        queue.pop();

        // break if finish vertex
        if (currentVertex == finish) {/*forward_end = true; */break;}

        // the vertex becomes processed
        is_processedMutex.lock();
        is_processed.insert(currentVertex);
        is_processedMutex.unlock();

        is_processed_rMutex.lock();
        if (is_processed_r.find(currentVertex) != is_processed_r.end()){
            centerMutex.lock();
            meetingsMutex.lock();
            dist_rMutex.lock();
            if (center == nullptr || dist[currentVertex] + dist_r[currentVertex] < dist[center] + dist_r[center]){
                center = currentVertex;
                meetings = 0;
                //std::cout << "new f center\n";
            }
            else if (++meetings > AMOUNT_OF_COMMON ) {
                centerMutex.unlock();
                meetingsMutex.unlock();
                dist_rMutex.unlock();
                is_processed_rMutex.unlock();
                break;
            };
            centerMutex.unlock();
            meetingsMutex.unlock();
            dist_rMutex.unlock();
        }
        is_processed_rMutex.unlock();

        // every non-processed incident vertex is going to the queue
        for (Edge* edge : currentVertex->incidentEdges){
            result->forwardSearchEdges.insert(edge);
            // getting next incident vertex
            Vertex* nextVertex = currentVertex == edge->vFrom ? edge->vTo : edge->vFrom;
            // if it's processed, skip it
            if (is_processed.find(nextVertex) != is_processed.end()) continue;
            // getting new dist
            double currentDist = dist[currentVertex] + edge->weight;
            // if it's not computed or less than already computed, change dist
            // if for the first time
            if (dist.find(nextVertex) == dist.end()){
                distMutex.lock();
                dist.insert({nextVertex, currentDist});
                distMutex.unlock();
                edge_to.insert({nextVertex, edge});
                queue.push(nextVertex);
            }
            else if (dist[nextVertex] > currentDist){
                distMutex.lock();
                dist[nextVertex] = currentDist;
                distMutex.unlock();
                edge_to[nextVertex] = edge;
            }
        }
    }
    reverse_thread.join();
    //// algorithm done, checking for path existence
    if (center == nullptr){
        result->time = double(clock() - time_start) / CLOCKS_PER_SEC;
        result->found = false;
        return result;
    }
    //// building the path
    // forward search part
    Vertex* currentVertex = center;
    result->path.push_back(center);
    do {
        //std::cout << "Building path: " << ++c << std::endl;
        Edge* currentEdge = edge_to[currentVertex];
        result->length += currentEdge->weight;
        //std::cout << currentEdge->weight << std::endl;
        //length_check += currentEdge->weight;
        result->forwardSearchPathEdges.insert(currentEdge);
        currentVertex = currentVertex == currentEdge->vFrom ? currentEdge->vTo : currentEdge->vFrom;
        result->path.push_front(currentVertex);
        //std::cout << "Current Vertex Id: " << currentVertex->id << std::endl;
    } while (currentVertex != start);

    // reverse search part
    currentVertex = center;
    do {
        //std::cout << "Building path: " << ++c << std::endl;
        Edge* currentEdge = edge_to_r[currentVertex];
        result->length += currentEdge->weight;
        //std::cout << currentEdge->weight << std::endl;
        //length_check += currentEdge->weight;
        result->reverseSearchPathEdges.insert(currentEdge);
        currentVertex = currentVertex == currentEdge->vFrom ? currentEdge->vTo : currentEdge->vFrom;
        result->path.push_back(currentVertex);
        //std::cout << "Current Vertex Id: " << currentVertex->id << std::endl;
    } while (currentVertex != finish);
    result->time = double(clock() - time_start) / CLOCKS_PER_SEC;
    for (Edge* edge : result->forwardSearchEdges)
        if (result->reverseSearchEdges.find(edge) != result->reverseSearchEdges.end())
            result->commonSearchEdges.insert(edge);
    return result;
}

const AlgResult* Algorithms::astar(const Graph* g, Vertex* start, Vertex* finish) {
    // initialization of result variable
    auto result = new AlgResult;

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
    result->path.push_front(finish);
    Vertex* currentVertex = finish;
    double length_check = 0;
    do {
        //std::cout << "Building path: " << ++c << std::endl;
        Edge* currentEdge = edge_to[currentVertex];
        result->length += currentEdge->weight;
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

const AlgResult*Algorithms::bastar(const Graph* g, Vertex* start, Vertex* finish) {
    const int AMOUNT_OF_COMMON = 40;
    //// initialization of result variable
    auto result = new AlgResult;

    //// initialization of working maps
    // forward search
    std::unordered_set<const Vertex*> is_processed;
    std::unordered_map<const Vertex*, double> dist;
    dist.insert({start, 0.0});
    std::unordered_map<const Vertex*, double> minDistToFinish;
    minDistToFinish.insert({start, evklidDistance(start, finish)});
    std::unordered_map<const Vertex*, Edge*> edge_to;
    // reverse search
    std::unordered_set<const Vertex*> is_processed_r;
    std::unordered_map<const Vertex*, double> dist_r;
    dist_r.insert({finish, 0.0});
    std::unordered_map<const Vertex*, double> minDistToStart;
    minDistToFinish.insert({finish, evklidDistance(start, finish)});
    std::unordered_map<const Vertex*, Edge*> edge_to_r;

    //// priority queues initialization
    auto comparator = [&dist, &minDistToFinish](const Vertex* l, const Vertex* r){
        return dist.find(l)->second + minDistToFinish.find(l)->second > dist.find(r)->second + minDistToFinish.find(r)->second;
    };
    auto comparator_r = [&dist_r, &minDistToStart](const Vertex* l, const Vertex* r){
        return dist_r.find(l)->second + minDistToStart.find(l)->second > dist_r.find(r)->second + minDistToStart.find(r)->second;

    };
    std::priority_queue<Vertex*, std::vector<Vertex*>, decltype(comparator)> queue(comparator);
    std::priority_queue<Vertex*, std::vector<Vertex*>, decltype(comparator_r)> queue_r(comparator_r);


    //// amount of meetings of 2 searches
    int meetings = 0;
    bool forward_end = false,
            reverse_end = false;
    Vertex* center = nullptr;

    //// official algorithm start
    time_t time_start = std::clock();
    queue.push(start);
    queue_r.push(finish);
    // while both queues are not empty and amount of meetings is less then 20
    //int c = 0;
    while (!(queue.empty() || queue_r.empty()) && meetings < AMOUNT_OF_COMMON ){
        //std::cout << ++c << std::endl;
        //// forward step
        // picking the closest vertex
        //std::cout << "Main Cycle: " << ++c << std::endl;
        Vertex* currentVertex = queue.top();
        queue.pop();

        // break if finish vertex
        if (currentVertex == finish) {forward_end = true; break;}

        // the vertex becomes processed
        is_processed.insert(currentVertex);

        if (is_processed_r.find(currentVertex) != is_processed_r.end()){
            if (center == nullptr || dist[currentVertex] + dist_r[currentVertex] < dist[center] + dist_r[center]){
                center = currentVertex;
                meetings = 0;
                //std::cout << "new f center\n";
            }
            else if (++meetings > AMOUNT_OF_COMMON ) break;
        }

        // every non-processed incident vertex is going to the queue
        for (Edge* edge : currentVertex->incidentEdges){
            result->forwardSearchEdges.insert(edge);
            // getting next incident vertex
            Vertex* nextVertex = currentVertex == edge->vFrom ? edge->vTo : edge->vFrom;
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

        //// reverse step
        // picking the closest vertex
        //std::cout << "Main Cycle: " << ++c << std::endl;
        currentVertex = queue_r.top();
        queue_r.pop();

        // break if finish vertex
        if (currentVertex == start) {reverse_end = true; break;}

        // the vertex becomes processed
        is_processed_r.insert(currentVertex);

        // checking for the meeting
        if (is_processed.find(currentVertex) != is_processed.end()){
            if (center == nullptr || dist[currentVertex] + dist_r[currentVertex] < dist[center] + dist_r[center]){
                center = currentVertex;
                meetings = 0;
                //std::cout << "new r center\n";
            }
            else if (++meetings > AMOUNT_OF_COMMON) break;
        }

        // every non-processed incident vertex is going to the queue
        for (Edge* edge : currentVertex->incidentEdges){
            result->reverseSearchEdges.insert(edge);
            // getting next incident vertex
            Vertex* nextVertex = currentVertex == edge->vFrom ? edge->vTo : edge->vFrom;
            // if it's processed, skip it
            if (is_processed_r.find(nextVertex) != is_processed_r.end()) continue;
            // getting new dist
            double currentDist = dist_r[currentVertex] + edge->weight;
            // if it's not computed or less than already computed, change dist
            // if for the first time
            if (dist_r.find(nextVertex) == dist_r.end()){
                dist_r.insert({nextVertex, currentDist});
                edge_to_r.insert({nextVertex, edge});
                minDistToStart.insert({nextVertex, evklidDistance(nextVertex, start)});
                queue_r.push(nextVertex);
            }
            else if (dist_r[nextVertex] > currentDist){
                dist_r[nextVertex] = currentDist;
                edge_to_r[nextVertex] = edge;
            }
        }
    }

    //// algorithm done, checking for path existence
    if (center == nullptr){
        result->time = double(clock() - time_start) / CLOCKS_PER_SEC;
        result->found = false;
        return result;
    }
    //// building the path
    // forward search part
    Vertex* currentVertex = center;
    result->path.push_back(center);
    do {
        //std::cout << "Building path: " << ++c << std::endl;
        Edge* currentEdge = edge_to[currentVertex];
        result->length += currentEdge->weight;
        //std::cout << currentEdge->weight << std::endl;
        //length_check += currentEdge->weight;
        result->forwardSearchPathEdges.insert(currentEdge);
        currentVertex = currentVertex == currentEdge->vFrom ? currentEdge->vTo : currentEdge->vFrom;
        result->path.push_front(currentVertex);
        //std::cout << "Current Vertex Id: " << currentVertex->id << std::endl;
    } while (currentVertex != start);

    // reverse search part
    currentVertex = center;
    do {
        //std::cout << "Building path: " << ++c << std::endl;
        Edge* currentEdge = edge_to_r[currentVertex];
        result->length += currentEdge->weight;
        //std::cout << currentEdge->weight << std::endl;
        //length_check += currentEdge->weight;
        result->reverseSearchPathEdges.insert(currentEdge);
        currentVertex = currentVertex == currentEdge->vFrom ? currentEdge->vTo : currentEdge->vFrom;
        result->path.push_back(currentVertex);
        //std::cout << "Current Vertex Id: " << currentVertex->id << std::endl;
    } while (currentVertex != finish);
    result->time = double(clock() - time_start) / CLOCKS_PER_SEC;
    for (Edge* edge : result->forwardSearchEdges)
        if (result->reverseSearchEdges.find(edge) != result->reverseSearchEdges.end())
            result->commonSearchEdges.insert(edge);
    return result;
}

//const AlgResult* Algorithms::pastar(const Graph* g, Vertex* start, Vertex* finish) {
//    const int AMOUNT_OF_COMMON = 10;
//    //// initialization of result variable
//    auto result = new AlgResult;
//
//    //// initialization of working maps
//    // forward search
//    std::unordered_set<const Vertex*> is_processed;
//    std::unordered_map<const Vertex*, double> dist;
//    dist.insert({start, 0.0});
//    std::unordered_map<const Vertex*, Edge*> edge_to;
//    std::unordered_map<const Vertex*, double> minDistToFinish;
//    // reverse search
//    std::unordered_set<const Vertex*> is_processed_r;
//    std::unordered_map<const Vertex*, double> dist_r;
//    dist_r.insert({finish, 0.0});
//    std::unordered_map<const Vertex*, Edge*> edge_to_r;
//    std::unordered_map<const Vertex*, double> minDistToStart;
//
//    //// priority queues initialization
//    auto comparator = [&dist, &minDistToFinish](const Vertex* l, const Vertex* r){
//        return dist.find(l)->second + minDistToFinish.find(l)->second > dist.find(r)->second + minDistToFinish.find(r)->second;
//    };
//    auto comparator_r = [&dist_r, &minDistToStart](const Vertex* l, const Vertex* r){
//        return dist_r.find(l)->second + minDistToStart.find(l)->second > dist_r.find(r)->second + minDistToStart.find(r)->second;
//    };
//    std::priority_queue<Vertex*, std::vector<Vertex*>, decltype(comparator)> queue(comparator);
//    std::priority_queue<Vertex*, std::vector<Vertex*>, decltype(comparator_r)> queue_r(comparator_r);
//
//
//    //// amount of meetings of 2 searches
//    int meetings = 0;
//    bool forward_end = false,
//            reverse_end = false;
//    Vertex* center = nullptr;
//
//    //// preparing thread for forward search
//    std::mutex centerMutex;
//    std::mutex meetingsMutex;
//    std::mutex is_processed_rMutex;
//    std::mutex is_processedMutex;
//    std::mutex dist_rMutex;
//    std::mutex distMutex;
//
//    auto forward_lambda = [&minDistToFinish, &start, &finish, &is_processed, &dist, &edge_to, &queue, &result, &is_processed_r, &center, &meetings, &dist_r,
//            &centerMutex, &meetingsMutex, &is_processed_rMutex, &is_processedMutex, &dist_rMutex, &distMutex](){
//        queue.push(start);
//        while (!queue.empty()){
//            Vertex* currentVertex = queue.top();
//            queue.pop();
//
//            // break if finish vertex
//            if (currentVertex == finish) {/*forward_end = true; */break;}
//
//            // the vertex becomes processed
//            is_processedMutex.lock();
//            is_processed.insert(currentVertex);
//            is_processedMutex.unlock();
//
//            is_processed_rMutex.lock();
//            if (is_processed_r.find(currentVertex) != is_processed_r.end()){
//                centerMutex.lock();
//                meetingsMutex.lock();
//                dist_rMutex.lock();
//                if (center == nullptr || dist[currentVertex] + dist_r[currentVertex] < dist[center] + dist_r[center]){
//                    center = currentVertex;
//                    meetings = 0;
//                    //std::cout << "new f center\n";
//                }
//                else if (++meetings > AMOUNT_OF_COMMON ) {
//                    centerMutex.unlock();
//                    meetingsMutex.unlock();
//                    dist_rMutex.unlock();
//                    is_processed_rMutex.unlock();
//                    break;
//                };
//                centerMutex.unlock();
//                meetingsMutex.unlock();
//                dist_rMutex.unlock();
//            }
//            is_processed_rMutex.unlock();
//
//            // every non-processed incident vertex is going to the queue
//            for (Edge* edge : currentVertex->incidentEdges){
//                result->forwardSearchEdges.insert(edge);
//                // getting next incident vertex
//                Vertex* nextVertex = currentVertex == edge->vFrom ? edge->vTo : edge->vFrom;
//                // if it's processed, skip it
//                if (is_processed.find(nextVertex) != is_processed.end()) continue;
//                // getting new dist
//                double currentDist = dist[currentVertex] + edge->weight;
//                // if it's not computed or less than already computed, change dist
//                // if for the first time
//                if (dist.find(nextVertex) == dist.end()){
//                    distMutex.lock();
//                    dist.insert({nextVertex, currentDist});
//                    distMutex.unlock();
//                    edge_to.insert({nextVertex, edge});
//                    minDistToFinish.insert({nextVertex, evklidDistance(nextVertex, finish)});
//                    queue.push(nextVertex);
//                }
//                else if (dist[nextVertex] > currentDist){
//                    distMutex.lock();
//                    dist[nextVertex] = currentDist;
//                    distMutex.unlock();
//                    edge_to[nextVertex] = edge;
//                }
//            }
//        }
//    };
//
//    auto reverse_lambda = [&minDistToStart, &start, &finish, &is_processed, &dist, &edge_to_r, &queue_r, &result, &is_processed_r, &center, &meetings, &dist_r,
//            &centerMutex, &meetingsMutex, &is_processed_rMutex, &is_processedMutex, &dist_rMutex, &distMutex](){
//        queue_r.push(finish);
//        while (!queue_r.empty()){
//            Vertex* currentVertex = queue_r.top();
//            queue_r.pop();
//
//            // break if finish vertex
//            if (currentVertex == start) {/*reverse_end = true;*/ break;}
//
//            // the vertex becomes processed
//            is_processed_rMutex.lock();
//            is_processed_r.insert(currentVertex);
//            is_processed_rMutex.unlock();
//
//            // checking for the meeting
//            is_processedMutex.lock();
//            if (is_processed.find(currentVertex) != is_processed.end()){
//                centerMutex.lock();
//                meetingsMutex.lock();
//                distMutex.lock();
//                if (center == nullptr || dist[currentVertex] + dist_r[currentVertex] < dist[center] + dist_r[center]){
//                    center = currentVertex;
//                    meetings = 0;
//                    //std::cout << "new r center\n";
//                }
//                else if (++meetings > AMOUNT_OF_COMMON) {
//                    centerMutex.unlock();
//                    meetingsMutex.unlock();
//                    distMutex.unlock();
//                    is_processedMutex.unlock();
//                    break;
//                }
//                centerMutex.unlock();
//                meetingsMutex.unlock();
//                distMutex.unlock();
//            }
//            is_processedMutex.unlock();
//
//            // every non-processed incident vertex is going to the queue
//            for (Edge* edge : currentVertex->incidentEdges){
//                result->reverseSearchEdges.insert(edge);
//                // getting next incident vertex
//                Vertex* nextVertex = currentVertex == edge->vFrom ? edge->vTo : edge->vFrom;
//                // if it's processed, skip it
//                if (is_processed_r.find(nextVertex) != is_processed_r.end()) continue;
//                // getting new dist
//                double currentDist = dist_r[currentVertex] + edge->weight;
//                // if it's not computed or less than already computed, change dist
//                // if for the first time
//                if (dist_r.find(nextVertex) == dist_r.end()){
//                    dist_rMutex.lock();
//                    dist_r.insert({nextVertex, currentDist});
//                    dist_rMutex.unlock();
//                    edge_to_r.insert({nextVertex, edge});
//                    minDistToStart.insert({nextVertex, evklidDistance(nextVertex, start)});
//                    queue_r.push(nextVertex);
//                }
//                else if (dist_r[nextVertex] > currentDist){
//                    dist_rMutex.lock();
//                    dist_r[nextVertex] = currentDist;
//                    dist_rMutex.unlock();
//                    edge_to_r[nextVertex] = edge;
//                }
//            }
//        }
//    };
//
//
//    //// official algorithm start
//    std::thread forward_thread(forward_lambda);
//    std::thread reverse_thread(reverse_lambda);
//    time_t time_start = clock();
//
//    forward_thread.join();
//    reverse_thread.join();
//    //// algorithm done, checking for path existence
//    if (center == nullptr){
//        result->time = double(clock() - time_start) / CLOCKS_PER_SEC;
//        result->found = false;
//        return result;
//    }
//    //// building the path
//    // forward search part
//    Vertex* currentVertex = center;
//    result->path.push_back(center);
//    do {
//        //std::cout << "Building path: " << ++c << std::endl;
//        Edge* currentEdge = edge_to[currentVertex];
//        result->length += currentEdge->weight;
//        //std::cout << currentEdge->weight << std::endl;
//        //length_check += currentEdge->weight;
//        result->forwardSearchPathEdges.insert(currentEdge);
//        currentVertex = currentVertex == currentEdge->vFrom ? currentEdge->vTo : currentEdge->vFrom;
//        result->path.push_front(currentVertex);
//        //std::cout << "Current Vertex Id: " << currentVertex->id << std::endl;
//    } while (currentVertex != start);
//
//    // reverse search part
//    currentVertex = center;
//    do {
//        //std::cout << "Building path: " << ++c << std::endl;
//        Edge* currentEdge = edge_to_r[currentVertex];
//        result->length += currentEdge->weight;
//        //std::cout << currentEdge->weight << std::endl;
//        //length_check += currentEdge->weight;
//        result->reverseSearchPathEdges.insert(currentEdge);
//        currentVertex = currentVertex == currentEdge->vFrom ? currentEdge->vTo : currentEdge->vFrom;
//        result->path.push_back(currentVertex);
//        //std::cout << "Current Vertex Id: " << currentVertex->id << std::endl;
//    } while (currentVertex != finish);
//    result->time = double(clock() - time_start) / CLOCKS_PER_SEC;
//    for (Edge* edge : result->forwardSearchEdges)
//        if (result->reverseSearchEdges.find(edge) != result->reverseSearchEdges.end())
//            result->commonSearchEdges.insert(edge);
//    return result;
//}

const AlgResult* Algorithms::pastar(const Graph* g, Vertex* start, Vertex* finish) {
    const int AMOUNT_OF_COMMON = 10;
    //// initialization of result variable
    auto result = new AlgResult;

    //// initialization of working maps
    // forward search
    std::unordered_set<const Vertex*> is_processed;
    std::unordered_map<const Vertex*, double> dist;
    dist.insert({start, 0.0});
    std::unordered_map<const Vertex*, Edge*> edge_to;
    std::unordered_map<const Vertex*, double> minDistToFinish;
    // reverse search
    std::unordered_set<const Vertex*> is_processed_r;
    std::unordered_map<const Vertex*, double> dist_r;
    dist_r.insert({finish, 0.0});
    std::unordered_map<const Vertex*, Edge*> edge_to_r;
    std::unordered_map<const Vertex*, double> minDistToStart;

    //// priority queues initialization
    auto comparator = [&dist, &minDistToFinish](const Vertex* l, const Vertex* r){
        return dist.find(l)->second + minDistToFinish.find(l)->second > dist.find(r)->second + minDistToFinish.find(r)->second;
    };
    auto comparator_r = [&dist_r, &minDistToStart](const Vertex* l, const Vertex* r){
        return dist_r.find(l)->second + minDistToStart.find(l)->second > dist_r.find(r)->second + minDistToStart.find(r)->second;
    };
    std::priority_queue<Vertex*, std::vector<Vertex*>, decltype(comparator)> queue(comparator);
    std::priority_queue<Vertex*, std::vector<Vertex*>, decltype(comparator_r)> queue_r(comparator_r);


    //// amount of meetings of 2 searches
    int meetings = 0;
    bool forward_end = false,
            reverse_end = false;
    Vertex* center = nullptr;

    //// preparing thread for forward search
    std::mutex centerMutex;
    std::mutex meetingsMutex;
    std::mutex is_processed_rMutex;
    std::mutex is_processedMutex;
    std::mutex dist_rMutex;
    std::mutex distMutex;

    auto reverse_lambda = [&minDistToStart, &start, &finish, &is_processed, &dist, &edge_to_r, &queue_r, &result, &is_processed_r, &center, &meetings, &dist_r,
            &centerMutex, &meetingsMutex, &is_processed_rMutex, &is_processedMutex, &dist_rMutex, &distMutex](){
        queue_r.push(finish);
        while (!queue_r.empty()){
            Vertex* currentVertex = queue_r.top();
            queue_r.pop();

            // break if finish vertex
            if (currentVertex == start) {/*reverse_end = true;*/ break;}

            // the vertex becomes processed
            is_processed_rMutex.lock();
            is_processed_r.insert(currentVertex);
            is_processed_rMutex.unlock();

            // checking for the meeting
            is_processedMutex.lock();
            if (is_processed.find(currentVertex) != is_processed.end()){
                centerMutex.lock();
                meetingsMutex.lock();
                distMutex.lock();
                if (center == nullptr || dist[currentVertex] + dist_r[currentVertex] < dist[center] + dist_r[center]){
                    center = currentVertex;
                    meetings = 0;
                    //std::cout << "new r center\n";
                }
                else if (++meetings > AMOUNT_OF_COMMON) {
                    centerMutex.unlock();
                    meetingsMutex.unlock();
                    distMutex.unlock();
                    is_processedMutex.unlock();
                    break;
                }
                centerMutex.unlock();
                meetingsMutex.unlock();
                distMutex.unlock();
            }
            is_processedMutex.unlock();

            // every non-processed incident vertex is going to the queue
            for (Edge* edge : currentVertex->incidentEdges){
                result->reverseSearchEdges.insert(edge);
                // getting next incident vertex
                Vertex* nextVertex = currentVertex == edge->vFrom ? edge->vTo : edge->vFrom;
                // if it's processed, skip it
                if (is_processed_r.find(nextVertex) != is_processed_r.end()) continue;
                // getting new dist
                double currentDist = dist_r[currentVertex] + edge->weight;
                // if it's not computed or less than already computed, change dist
                // if for the first time
                if (dist_r.find(nextVertex) == dist_r.end()){
                    dist_rMutex.lock();
                    dist_r.insert({nextVertex, currentDist});
                    dist_rMutex.unlock();
                    edge_to_r.insert({nextVertex, edge});
                    minDistToStart.insert({nextVertex, evklidDistance(nextVertex, start)});
                    queue_r.push(nextVertex);
                }
                else if (dist_r[nextVertex] > currentDist){
                    dist_rMutex.lock();
                    dist_r[nextVertex] = currentDist;
                    dist_rMutex.unlock();
                    edge_to_r[nextVertex] = edge;
                }
            }
        }
    };


    //// official algorithm start
    std::thread reverse_thread(reverse_lambda);
    time_t time_start = clock();

    queue.push(start);
    while (!queue.empty()){
        Vertex* currentVertex = queue.top();
        queue.pop();

        // break if finish vertex
        if (currentVertex == finish) {/*forward_end = true; */break;}

        // the vertex becomes processed
        is_processedMutex.lock();
        is_processed.insert(currentVertex);
        is_processedMutex.unlock();

        is_processed_rMutex.lock();
        if (is_processed_r.find(currentVertex) != is_processed_r.end()){
            centerMutex.lock();
            meetingsMutex.lock();
            dist_rMutex.lock();
            if (center == nullptr || dist[currentVertex] + dist_r[currentVertex] < dist[center] + dist_r[center]){
                center = currentVertex;
                meetings = 0;
                //std::cout << "new f center\n";
            }
            else if (++meetings > AMOUNT_OF_COMMON ) {
                centerMutex.unlock();
                meetingsMutex.unlock();
                dist_rMutex.unlock();
                is_processed_rMutex.unlock();
                break;
            };
            centerMutex.unlock();
            meetingsMutex.unlock();
            dist_rMutex.unlock();
        }
        is_processed_rMutex.unlock();

        // every non-processed incident vertex is going to the queue
        for (Edge* edge : currentVertex->incidentEdges){
            result->forwardSearchEdges.insert(edge);
            // getting next incident vertex
            Vertex* nextVertex = currentVertex == edge->vFrom ? edge->vTo : edge->vFrom;
            // if it's processed, skip it
            if (is_processed.find(nextVertex) != is_processed.end()) continue;
            // getting new dist
            double currentDist = dist[currentVertex] + edge->weight;
            // if it's not computed or less than already computed, change dist
            // if for the first time
            if (dist.find(nextVertex) == dist.end()){
                distMutex.lock();
                dist.insert({nextVertex, currentDist});
                distMutex.unlock();
                edge_to.insert({nextVertex, edge});
                minDistToFinish.insert({nextVertex, evklidDistance(nextVertex, finish)});
                queue.push(nextVertex);
            }
            else if (dist[nextVertex] > currentDist){
                distMutex.lock();
                dist[nextVertex] = currentDist;
                distMutex.unlock();
                edge_to[nextVertex] = edge;
            }
        }
    }

    reverse_thread.join();
    //// algorithm done, checking for path existence
    if (center == nullptr){
        result->time = double(clock() - time_start) / CLOCKS_PER_SEC;
        result->found = false;
        return result;
    }
    //// building the path
    // forward search part
    Vertex* currentVertex = center;
    result->path.push_back(center);
    do {
        //std::cout << "Building path: " << ++c << std::endl;
        Edge* currentEdge = edge_to[currentVertex];
        result->length += currentEdge->weight;
        //std::cout << currentEdge->weight << std::endl;
        //length_check += currentEdge->weight;
        result->forwardSearchPathEdges.insert(currentEdge);
        currentVertex = currentVertex == currentEdge->vFrom ? currentEdge->vTo : currentEdge->vFrom;
        result->path.push_front(currentVertex);
        //std::cout << "Current Vertex Id: " << currentVertex->id << std::endl;
    } while (currentVertex != start);

    // reverse search part
    currentVertex = center;
    do {
        //std::cout << "Building path: " << ++c << std::endl;
        Edge* currentEdge = edge_to_r[currentVertex];
        result->length += currentEdge->weight;
        //std::cout << currentEdge->weight << std::endl;
        //length_check += currentEdge->weight;
        result->reverseSearchPathEdges.insert(currentEdge);
        currentVertex = currentVertex == currentEdge->vFrom ? currentEdge->vTo : currentEdge->vFrom;
        result->path.push_back(currentVertex);
        //std::cout << "Current Vertex Id: " << currentVertex->id << std::endl;
    } while (currentVertex != finish);
    result->time = double(clock() - time_start) / CLOCKS_PER_SEC;
    for (Edge* edge : result->forwardSearchEdges)
        if (result->reverseSearchEdges.find(edge) != result->reverseSearchEdges.end())
            result->commonSearchEdges.insert(edge);
    return result;
}

// attended
double Algorithms::evklidDistance(const Vertex *a, const Vertex *b) {
    double sq_d_lon = pow(a->lon - b->lon, 2);
    double sq_d_lat = pow(a->lat - b->lat, 2);
    return sqrt(sq_d_lat + sq_d_lon);
}