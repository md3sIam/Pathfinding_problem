#pragma once

#include <string>

class Vertex;

class Edge {

private:
    static unsigned long max_id;
    unsigned long id;
    Vertex *vFrom;
    Vertex *vTo;
    long double weight;

    //STATIC METHODS
    static std::tuple<long, long, long double> get_edge_info_from_csv_string(const std::string& str);

public:
    Edge(Vertex *from, Vertex *to, long double weight);
    Edge(const Edge& e);
    Edge(Edge&& e) noexcept;
    Edge& operator=(const Edge &e);
    Edge& operator=(Edge &&e) noexcept;
    ~Edge();

    std::string get_info();

    friend class Graph;
};
