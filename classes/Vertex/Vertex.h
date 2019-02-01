#pragma once

#include <vector>
#include <string>
#include <tuple>

class Edge;

class Vertex {

private:
    //Variables
    static long max_custom_id;
    long id;
    double x, y;
    std::vector<Edge*> incidentEdges;

    //STATIC METHODS
    static Vertex* get_vertex_from_csv_string(const std::string &str);

public:
    Vertex(long id, double x, double y);
    Vertex(Vertex const &v);
    Vertex(Vertex&& v) noexcept;
    Vertex& operator=(const Vertex& v);
    Vertex& operator=(Vertex&& v) noexcept;

    ~Vertex();

    std::string get_info();

    friend class Edge;
    friend class Graph;
};