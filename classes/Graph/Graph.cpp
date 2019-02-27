#include <cstdio>
#include <fstream>
#include <iostream>
#include <cmath>
#include "Graph.h"
#include "../Vertex/Vertex.h"
#include "../Edge/Edge.h"

Graph::Graph(): isNormalized(false), maxX(-181), maxY(-181), minX(181), minY(181) {}

Graph::Graph(const Graph &g) {
    vertices = g.vertices;
    edges = g.edges;
    isNormalized = g.isNormalized;
}

Graph::Graph(Graph &&g) noexcept{
    vertices = std::move(g.vertices);
    edges = std::move(g.edges);
    isNormalized = g.isNormalized;
}

Graph& Graph::operator=(const Graph &g) {
    vertices = g.vertices;
    edges = g.edges;
    isNormalized = g.isNormalized;
    return *this;
}

Graph& Graph::operator=(Graph &&g) noexcept {
    vertices = std::move(g.vertices);
    edges = std::move(g.edges);
    isNormalized = g.isNormalized;
    return *this;
}

Graph::~Graph() {

    for (std::pair<unsigned long, Edge*> e : edges){
        delete e.second;
    }
    std::map<unsigned long, Edge*>().swap(edges);

    for (std::pair<long, Vertex*> v : vertices){
        delete v.second;
    }
    std::map<long, Vertex*>().swap(vertices);
}

int Graph::read_graph_from_files(std::string vFilename, std::string eFilename) {

    try {
        read_vertices(vFilename);
        read_edges(eFilename);
        normalize();
    }

    catch (std::exception &ex){
        printf("%s\n", ex.what());
        delete this;
        return 0;
    }
    return 1;
}

void Graph::read_vertices(const std::string &filename) {
    std::ifstream fs(filename);
    if (!fs){
        std::string msg("File not found: ");
        msg += filename + '\n';
        throw std::runtime_error(msg);
    }
    std::string s;
    std::getline(fs, s);
    while (std::getline(fs, s)){
        Vertex* vertex = Vertex::get_vertex_from_csv_string(s);
        vertices.insert(std::make_pair(vertex->id, vertex));
        checkForMaxMin(*vertex);
    }
    fs.close();
}

void Graph::checkForMaxMin(Vertex &v){
    if (maxX < v.lon){
        maxX = v.lon;
    }
    if (maxY < v.lat){
        maxY = v.lat;
    }
    if (minX > v.lon){
        minX = v.lon;
    }
    if (minY > v.lat){
        minY = v.lat;
    }
}

void Graph::read_edges(const std::string &filename) {
    std::ifstream fs(filename);
    if (!fs){
        std::string msg("File not found: ");
        msg += filename + '\n';
        throw std::runtime_error(msg);
    }
    std::string s;
    std::getline(fs, s);
    while (std::getline(fs, s)){
        std::tuple<unsigned long, long, long, long double> edge_info = Edge::get_edge_info_from_csv_string(s);
        unsigned long id = std::get<0>(edge_info);
        Vertex &v1 = get_vertex_by_id(std::get<1>(edge_info));
        Vertex &v2 = get_vertex_by_id(std::get<2>(edge_info));
        long double weight = std::get<3>(edge_info);
        Edge* edge = new Edge(id, &v1, &v2, weight);
        edges.insert({edge->id, edge});
    }
    fs.close();
}

void Graph::normalize() {
    for (auto &pair : edges){
        pair.second->vFrom->incidentEdges.push_back(pair.second);
        pair.second->vTo->incidentEdges.push_back(pair.second);
    }
}

Vertex& Graph::get_vertex_by_id(long id) {
    Vertex* v = vertices.find(id)->second;
    return *v;
}

Vertex* Graph::getTheClosestVertex(float x, float y, float radius) {
    Vertex* res = nullptr;
    float minDist = radius * radius + 1;
    for (auto v : vertices){
        float dist = (v.second->lon - x) * (v.second->lon - x) +
                   (v.second->lat - y) * (v.second->lat - y);
        if (dist < minDist){
            res = v.second;
            minDist = dist;
        }
    }
    return res;
}

void Graph::get_info() {
    for (auto pair : vertices){
        std::cout << pair.second->get_info() << "\n\n";
    }

    for (auto pair : edges){
        std::cout << pair.second->get_info() << "\n\n";
    }

    std::cout << "\nTOTAL\nVertices: " << vertices.size();
    std::cout << "\nEdges: " << edges.size() << '\n';
}

void Graph::addVertex(Vertex *vertex) {
    vertex->id = (long)((vertex->lon + vertex->lat) * 1000000);
    bool isIdOk;
    bool notify = false;
    long expectedId = vertex->id;
    do {
        isIdOk = true;
        if (vertices.find(vertex->id) != vertices.end()) {
            isIdOk = false;
            vertex->id++;
            notify = true;
        }
    } while (!isIdOk);
    if (notify) {
        std::cout << "Upon vertex creation expected ID {" <<
            expectedId << "} was changed on {" << vertex->id << "}" << std::endl;
    }
    vertices.insert({vertex->id, vertex});
}

void Graph::addEdge(Edge *edge) {
    //if weight equals -1 then calculate weight in euclid space
    if (edge->weight == -1){
        edge->weight = sqrt((edge->vFrom->lon - edge->vTo->lon) *
                                    (edge->vFrom->lon - edge->vTo->lon) +
                                    (edge->vFrom->lat - edge->vTo->lat) *
                                            (edge->vFrom->lat - edge->vTo->lat));
    }
    bool isIdOk;
    bool notify = false;
    unsigned long expectedId = edge->id;
    do {
        isIdOk = true;
        if (edges.find(edge->id) != edges.end()){
            isIdOk = false;
            notify = true;
            edge->id++;
        }
    } while (!isIdOk);
    if (notify) {
        Edge::max_id += edge->id - expectedId; //stabilize
        std::cout << "Upon edge creation expected ID {" <<
                  expectedId << "} was changed on {" << edge->id << "}" << std::endl;
    }
    edges.insert({edge->id, edge});
}