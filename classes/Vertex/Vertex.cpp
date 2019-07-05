//
// Created by md3s on 25.01.19.
//

#include "Vertex.h"
#include <cstdio>
#include <iostream>
#include "../Utilities/CSVUtil/CSVUtil.h"

long Vertex::last_created_id = 0;

Vertex::Vertex(unsigned long id, double x, double y): GraphItem(id), lon(x), lat(y) {
    //printf("Vertex with id %ld created", id);
    this->id = id;
};

Vertex::Vertex(Vertex const &v): GraphItem(v.id){
    lon = v.lon;
    lat = v.lat;
    incidentEdges = v.incidentEdges;
    printf("Vertex with id %ld created", id);
}

Vertex::Vertex(Vertex && v) noexcept : GraphItem(v.id) {
    lon = v.lon;
    lat = v.lat;
    incidentEdges = v.incidentEdges;
}

Vertex& Vertex::operator=(const Vertex &v) {
    id = v.id;
    lon = v.lon;
    lat = v.lat;
    incidentEdges = v.incidentEdges;
    return *this;
}

Vertex& Vertex::operator=(Vertex &&v) noexcept {
    id = v.id;
    lon = v.lon;
    lat = v.lat;
    incidentEdges = std::move(v.incidentEdges);
    return *this;
}

Vertex::~Vertex() {
    std::vector<Edge*>().swap(incidentEdges);
}

std::string Vertex::get_info() const {
    std::string info;
    info += std::string("ID: ") + std::to_string(id);
    info += std::string("\nx: ") + std::to_string(lon);
    info += std::string("\ny: ") + std::to_string(lat);
    info += std::string("\nIE amount: ") + std::to_string(incidentEdges.size());
    return info;
}

Vertex* Vertex::get_vertex_from_csv_string(const std::string &str) {
    std::vector<std::string> vector = CSVUtil::parse_to_strings(str);
    CSVUtil::csv_to_cpp_fp(vector[1]);
    CSVUtil::csv_to_cpp_fp(vector[2]);

    unsigned long id = std::stoul(vector[0]);
    double x = std::stod(vector[2]);
    double y = std::stod(vector[1]);

    return new Vertex(id, x, y);
}

