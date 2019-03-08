#pragma once

#include <vector>
#include <string>
#include <map>

class Vertex;
class Edge;

class Graph {

    friend class CustomOpenGLWidget;

private:
    //PROPERTIES
    std::map<long, Vertex*> vertices;
    std::map<unsigned long, Edge*> edges;
    double maxX, maxY, minX, minY;

    bool isNormalized;
    //END OF PROPERTIES

    //METHODS
    void read_vertices(const std::string &filename);
    void read_edges(const std::string &filename);
    void normalize();
    void checkForMaxMin(Vertex &v);

    Vertex& get_vertex_by_id(long id);
    //END OF METHODS

public:
    Graph();
    Graph(const Graph& g);
    Graph(Graph&& g) noexcept;
    Graph& operator=(const Graph& g);
    Graph& operator=(Graph&& g) noexcept;
    ~Graph();

    void addVertex(Vertex* vertex);
    void addEdge(Edge* edge);

    void removeVertex(Vertex* v);
    void removeEdge(Edge* e);

    std::vector<Vertex*> getIncidentVertices(Vertex* v);

    int read_graph_from_files(std::string, std::string);
    void read_binary(const std::string &filename);

    void save_to_binary(const std::string &filename);

    Vertex* getTheClosestVertex(float x, float y, float radius = 100);
    Edge* getTheClosestEdge(float x, float y, float radius = 100);

    void get_info();

};
