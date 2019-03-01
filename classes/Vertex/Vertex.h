#pragma once

#include <vector>
#include <string>
#include <tuple>

class Edge;

class Vertex {

private:
    //Variables
    static long last_created_id;

    long id;
    double lat, lon;
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

    std::string get_info() const;

    friend class Edge;
    friend class Graph;
    friend class CustomOpenGLWidget;
};
