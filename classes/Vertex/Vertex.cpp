//
// Created by md3s on 25.01.19.
//

#include "Vertex.h"
#include <cstdio>
#include <iostream>
#include "../Utilities/CSVUtil/CSVUtil.h"

long Vertex::max_custom_id = 0;

Vertex::Vertex(long id, double x, double y): id(id), x(x), y(y) {
    //printf("Vertex with id %ld created", id);
};

Vertex::Vertex(Vertex const &v){
    id = v.id;
    x = v.x;
    y = v.y;
    incidentEdges = v.incidentEdges;
    printf("Vertex with id %ld created", id);
}

Vertex::Vertex(Vertex && v) noexcept {
    id = v.id;
    x = v.x;
    y = v.y;
    incidentEdges = v.incidentEdges;
}

Vertex& Vertex::operator=(const Vertex &v) {
    id = v.id;
    x = v.x;
    y = v.y;
    incidentEdges = v.incidentEdges;
    return *this;
}

Vertex& Vertex::operator=(Vertex &&v) noexcept {
    id = v.id;
    x = v.x;
    y = v.y;
    incidentEdges = std::move(v.incidentEdges);
    return *this;
}

Vertex::~Vertex() {
    std::vector<Edge*>().swap(incidentEdges);
}

std::string Vertex::get_info() {
    std::string info;
    info += std::string("ID: ") + std::to_string(id);
    info += std::string("\nx: ") + std::to_string(x);
    info += std::string("\ny: ") + std::to_string(y);
    info += std::string("\nIE amount: ") + std::to_string(incidentEdges.size());
    return info;
}

Vertex* Vertex::get_vertex_from_csv_string(const std::string &str) {
    std::vector<std::string> vector = CSVUtil::parse_to_strings(str);

    long id = std::stol(vector[0]);
    double x = std::stod(vector[1]);
    double y = std::stod(vector[2]);

    return new Vertex(id, x, y);
}

