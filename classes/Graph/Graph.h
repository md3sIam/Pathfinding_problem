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
    std::vector<Edge*> edges;
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
    Graph&operator=(Graph&& g) noexcept;
    ~Graph();

    int read_graph_from_files(std::string, std::string);
    Vertex* getTheClosestVertex(float x, float y, float radius = 100);

    void get_info();

};
