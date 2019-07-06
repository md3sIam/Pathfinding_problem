#include <cstdio>
#include <fstream>
#include <iostream>
#include <cmath>
#include <classes/Exceptions.h>
#include "Graph.h"
#include "../Vertex/Vertex.h"
#include "../Edge/Edge.h"

Graph::Graph(): isNormalized(false), maxX(100), maxY(100), minX(0), minY(0) {}

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

    for (std::pair<unsigned long, Vertex*> v : vertices){
        delete v.second;
    }
    std::map<unsigned long, Vertex*>().swap(vertices);
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
        msg += filename + "\n";
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
    if (maxX < v.lon || !isSetMaxX){
        maxX = v.lon;
        if (!isSetMaxX) isSetMaxX = true;
    }
    if (maxY < v.lat || !isSetMaxY){
        maxY = v.lat;
        if (!isSetMaxY) isSetMaxY = true;

    }
    if (minX > v.lon || !isSetMinX){
        minX = v.lon;
        if (!isSetMinX) isSetMinX = true;

    }
    if (minY > v.lat || !isSetMinY){
        minY = v.lat;
        if (!isSetMinY) isSetMinY = true;

    }
}

void Graph::read_edges(const std::string &filename) {
    std::ifstream fs(filename);
    if (!fs){
        std::string msg("File not found: ");
        msg += filename + "\n";
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
    float minDist = radius * radius;
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

Edge* Graph::getTheClosestEdge(float x, float y, float r) {
    Edge* res = nullptr;
    Vertex* closestVertex = getTheClosestVertex(x, y, r);
    float minDist = -1;
    for (auto it = closestVertex->incidentEdges.begin(); it != closestVertex->incidentEdges.end(); it++){
        Vertex* v = closestVertex == (*it)->vFrom ? (*it)->vTo : (*it)->vFrom;
        float dist = (v->lon - x) * (v->lon - x) +
                     (v->lat - y) * (v->lat - y);
        if (minDist == -1 || minDist > dist){
            res = *it;
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
    edge->vTo->incidentEdges.push_back(edge);
    edge->vFrom->incidentEdges.push_back(edge);
}

std::vector<Vertex*> Graph::getIncidentVertices(Vertex *v) {
    std::vector<Vertex*> result;
    for (auto edge : v->incidentEdges){
        Vertex* vToAdd = v == edge->vTo ? edge->vFrom : edge->vTo;
    }
}

void Graph::removeVertex(Vertex *v) {
    //REMOVING THIS WAY BECAUSE ITERATOR GANDONITSYA
    std::vector<Edge*> toremove;
    for (auto it = v->incidentEdges.begin(); it != v->incidentEdges.end(); it++){
        //Removing edges from vectors of incident vertices for given vertex
        toremove.push_back(*it);
    }
    for (Edge* edge : toremove){
        removeEdge(edge);
    }
    vertices.erase(vertices.find(v->id));
    delete v;
}

void Graph::removeEdge(Edge *e) {
    //Removing edge from vector of incident edges of vertex-from
    Vertex* vertex = e->vFrom;
    for (auto it = vertex->incidentEdges.begin(); it != vertex->incidentEdges.end(); it++){
        if ((*it) == e){
            vertex->incidentEdges.erase(it);
            break;
        }
    }
    //Removing edge from vector of incident edges of vertex-to
    vertex = e->vTo;
    for (auto it = vertex->incidentEdges.begin(); it != vertex->incidentEdges.end(); it++){
        if ((*it) == e){
            vertex->incidentEdges.erase(it);
            break;
        }
    }
    //Removing edge
    //std::cout << edges.size() << std::endl;
    edges.erase(e->id);
    //std::cout << edges.size() << std::endl;
    delete e;
}

void Graph::read_binary(const std::string &filename) {
    std::ifstream file;
    file.open(filename, std::ios::in | std::ios::binary);

    if (!file.is_open()){
        std::string msg;
        msg = "File \"" + filename + "\" is unavailable";
        throw FileNotOpenedException(msg.c_str());
    }

    char memblock[sizeof(long) + 2 * sizeof(double)];
    long id;
    double lon, lat;
    do {
        file.read(memblock, sizeof(memblock));
        id = *(long*)memblock;
        lat = *(double*)(memblock + sizeof(long));
        lon = *(double*)(memblock + sizeof(long) + sizeof(double));
        if (!(id == 0 && lon == 0 && lat == 0)){
            auto vertex = new Vertex(id, lon, lat);
            vertices.insert({id, vertex});
            checkForMaxMin(*vertex);
        }
    } while (!(id == 0 && lon == 0 && lat == 0));

    //reading edges
    char memblock2[3 * sizeof(long) + sizeof(double)];
    unsigned long id2;
    long idfrom, idto;
    double weight;
    int c = 0;
    do {
        file.read(memblock2, sizeof(memblock2));
        id2 = *(unsigned long*)memblock2;
        idfrom = *(long*)(memblock2 + sizeof(unsigned long));
        idto = *(long*)(memblock2 + sizeof(unsigned long) + sizeof(long));
        weight = *(double *)(memblock2 + sizeof(unsigned long) + 2 * sizeof(long));

        //
//        unsigned long wl = *(unsigned long *)(memblock2 + sizeof(unsigned long) + 2 * sizeof(long));
//        for (int i = sizeof(unsigned long) * 8 - 1; i >= 0; i--){
//            printf("%ld", ((wl & ((unsigned long)1 << i)) >> i));
//        }
//        std::cout << std::endl;
        //

        if (weight < 0) c++;
        if (!(id2 == 0 && idfrom == 0 && idto == 0 && weight == 0)){
            auto edge = new Edge(id2, vertices[idfrom], vertices[idto], weight);
            edges.insert({id2, edge});
        }
    } while (!(id2 == 0 && idfrom == 0 && idto == 0 && weight == 0));
    std::cout << "Amount of negative weight weights (wtf): " << c << '\n';
    file.close();
    normalize();
}

void Graph::save_to_binary(const std::string &filename) {
    std::ofstream file;
    file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!file.is_open()){
        std::string msg;
        msg = "File \"" + filename + "\" is unavailable";
        throw FileNotOpenedException(msg.c_str());
    }

    //Writing vertices
    char* memblock_long;
    char* memblock_double;
    for (auto pair : vertices){
        memblock_long = (char*)&(pair.second->id);
        file.write(memblock_long, sizeof(long));
        memblock_double = (char*)&(pair.second->lat);
        file.write(memblock_double, sizeof(double));
        memblock_double = (char*)&(pair.second->lon);
        file.write(memblock_double, sizeof(double));
    }

    //Writing separator 1 (vertices | edges)
    char byte_separator1[sizeof(long) + 2 * sizeof(double)]{0};
    file.write(byte_separator1, sizeof(byte_separator1));

    //Writing edges
    for (auto pair : edges){
        memblock_long = (char*)&(pair.second->id);
        file.write(memblock_long, sizeof(unsigned long));
        memblock_long = (char*)&(pair.second->vFrom->id);
        file.write(memblock_long, sizeof(long));
        memblock_long = (char*)&(pair.second->vTo->id);
        file.write(memblock_long, sizeof(long));
        memblock_double = (char*)&(pair.second->weight);
        file.write(memblock_double, sizeof(double));
    }

    //Writing separator 2 ( edges | features)
    char byte_separator2[sizeof(unsigned long) + 2 * sizeof(long) + sizeof(double)]{0};
    file.write(byte_separator2, sizeof(byte_separator2));

    file.close();
}