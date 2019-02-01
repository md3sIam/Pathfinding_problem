#pragma once

#include <vector>
#include <string>
#include <map>

class Vertex;
class Edge;

class Graph {

private:
    //PROPERTIES
    std::map<long, Vertex*> vertices;
    std::vector<Edge*> edges;

    bool isNormalized;
    //END OF PROPERTIES

    //METHODS
    void read_vertices(const std::string &filename);
    void read_edges(const std::string &filename);
    void normalize();

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

    void get_info();

};
