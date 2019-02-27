#include <vector>
#include <tuple>
#include <string>
#include "../Utilities/CSVUtil/CSVUtil.h"
#include "Edge.h"
#include "../Vertex/Vertex.h"

unsigned long Edge::max_id = 0;

Edge::Edge(unsigned long id, Vertex *from, Vertex *to, long double weight):
                        id(id), vFrom(from), vTo(to), weight(weight) {
    max_id++;
}

Edge::Edge(Vertex *from, Vertex *to, long double weight):
        id(max_id), vFrom(from), vTo(to), weight(weight) {
    max_id++;
}

Edge::Edge(const Edge &e) {
    id = e.id;
    vFrom = e.vFrom;
    vTo = e.vTo;
    weight = e.weight;
}

Edge::Edge(Edge &&e) noexcept {
    id = e.id;
    vFrom = e.vFrom;
    vTo = e.vTo;
    weight = e.weight;
}

Edge& Edge::operator=(const Edge &e) {
    id = e.id;
    vFrom = e.vFrom;
    vTo = e.vTo;
    weight = e.weight;
    return *this;
}

Edge& Edge::operator=(Edge &&e) noexcept {
    id = e.id;
    vFrom = e.vFrom;
    vTo = e.vTo;
    weight = e.weight;
    return *this;
}

Edge::~Edge() =default;

std::tuple<unsigned long, long, long, long double> Edge::get_edge_info_from_csv_string(const std::string &str) {
    std::vector<std::string> vector = CSVUtil::parse_to_strings(str);

    unsigned long id = std::stoul(vector[0]);
    long idFrom = std::stol(vector[1]);
    long idTo = std::stol(vector[2]);
    long double weight = std::stold(vector[3]);

    return std::make_tuple(id, idFrom, idTo, weight);
}

std::string Edge::get_info() {
    std::string info;

    info += std::string("From: ") + std::to_string(vFrom->id);
    info += std::string("\nTo: ") + std::to_string(vTo->id);
    info += std::string("\nWeight: ") + std::to_string(weight);
    return info;
}
