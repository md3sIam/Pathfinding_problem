#include <vector>
#include <tuple>
#include <string>
#include "../Utilities/CSVUtil/CSVUtil.h"
#include "Edge.h"
#include "../Vertex/Vertex.h"

Edge::Edge(Vertex *from, Vertex *to, long double weight): vFrom(from), vTo(to), weight(weight) {}

Edge::Edge(const Edge &e) {
    vFrom = e.vFrom;
    vTo = e.vTo;
    weight = e.weight;
}

Edge::Edge(Edge &&e) noexcept {
    vFrom = e.vFrom;
    vTo = e.vTo;
    weight = e.weight;
}

Edge& Edge::operator=(const Edge &e) {
    vFrom = e.vFrom;
    vTo = e.vTo;
    weight = e.weight;
    return *this;
}

Edge& Edge::operator=(Edge &&e) noexcept {
    vFrom = e.vFrom;
    vTo = e.vTo;
    weight = e.weight;
    return *this;
}

Edge::~Edge() =default;

std::tuple<long, long, long double> Edge::get_edge_info_from_csv_string(const std::string &str) {
    std::vector<std::string> vector = CSVUtil::parse_to_strings(str);

    long idFrom = std::stol(vector[0]);
    long idTo = std::stol(vector[1]);
    long double weight = std::stold(vector[2]);

    return std::make_tuple(idFrom, idTo, weight);
}

std::string Edge::get_info() {
    std::string info;

    info += std::string("From: ") + std::to_string(vFrom->id);
    info += std::string("\nTo: ") + std::to_string(vTo->id);
    info += std::string("\nWeight: ") + std::to_string(weight);
    return info;
}
