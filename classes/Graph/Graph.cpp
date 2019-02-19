#include <cstdio>
#include <fstream>
#include <iostream>
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

    for (Edge* e : edges){
        delete e;
    }
    std::vector<Edge*>().swap(edges);

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
    if (maxX < v.x){
        maxX = v.x;
    }
    if (maxY < v.y){
        maxY = v.y;
    }
    if (minX > v.x){
        minX = v.x;
    }
    if (minY > v.y){
        minY = v.y;
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
        std::tuple<long, long, long double> edge_info = Edge::get_edge_info_from_csv_string(s);
        Vertex &v1 = get_vertex_by_id(std::get<0>(edge_info));
        Vertex &v2 = get_vertex_by_id(std::get<1>(edge_info));
        long double weight = std::get<2>(edge_info);
        Edge* edge = new Edge(&v1, &v2, weight);
        edges.push_back(edge);
    }
    fs.close();
}

void Graph::normalize() {
    for (auto edge : edges){
        edge->vFrom->incidentEdges.push_back(edge);
        edge->vTo->incidentEdges.push_back(edge);
    }
}

Vertex& Graph::get_vertex_by_id(long id) {
    Vertex* v = vertices.find(id)->second;
    return *v;
}

void Graph::get_info() {
    for (auto pair : vertices){
        std::cout << pair.second->get_info() << "\n\n";
    }

    for (auto edge : edges){
        std::cout << edge->get_info() << "\n\n";
    }

    std::cout << "\nTOTAL\nVertices: " << vertices.size();
    std::cout << "\nEdges: " << edges.size() << '\n';
}

float* Graph::getEdgesPreparedToDraw() const {
    auto* result = new float[4 * edges.size()];
    int i = 0;
    for (auto edge = edges.begin(); edge != edges.end(); edge++, i += 4){
        result[i] = (float) (*edge)->vFrom->x;
        result[i + 1] = (float) (*edge)->vFrom->y;
        result[i + 2] = (float) (*edge)->vTo->x;
        result[i + 3] = (float) (*edge)->vTo->y;
    }
    return result;
}