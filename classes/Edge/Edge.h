#pragma once

#include <string>
#include <classes/GraphItem.h>

class Vertex;

class Edge : public GraphItem{

private:
    static unsigned long max_id;
    Vertex *vFrom;
    Vertex *vTo;
    double weight;

    //STATIC METHODS
    static std::tuple<unsigned long, long, long, long double> get_edge_info_from_csv_string(const std::string& str);

public:
    Edge(Vertex *from, Vertex *to, long double weight);
    Edge(unsigned long id, Vertex *from, Vertex *to, long double weight);
    Edge(const Edge& e);
    Edge(Edge&& e) noexcept;
    Edge& operator=(const Edge &e);
    Edge& operator=(Edge &&e) noexcept;
    ~Edge();

    std::string get_info() const override;

    friend class Graph;
    friend class CustomOpenGLWidget;
    template <class T> friend class Category;
    friend class Algorithms;
};
