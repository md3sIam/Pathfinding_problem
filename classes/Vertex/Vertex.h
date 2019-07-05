#pragma once

#include <vector>
#include <string>
#include <tuple>
#include <classes/GraphItem.h>

class Edge;

class Vertex : public GraphItem {

private:
    //Variables
    static long last_created_id;

//    unsigned long id;
    double lat, lon;
    std::vector<Edge*> incidentEdges;

    //STATIC METHODS
    static Vertex* get_vertex_from_csv_string(const std::string &str);

public:
    Vertex(unsigned long id, double x, double y);
    Vertex(Vertex const &v);
    Vertex(Vertex&& v) noexcept;
    Vertex& operator=(const Vertex& v);
    Vertex& operator=(Vertex&& v) noexcept;

    ~Vertex();

    std::string get_info() const override ;

    friend class Edge;
    friend class Graph;
    friend class CustomOpenGLWidget;
    template <class T> friend class Category;
    friend class Algorithms;
};
