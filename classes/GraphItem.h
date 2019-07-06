//
// Created by md3s on 05.07.19.
//

#ifndef PATHFINDING_PROBLEM_GRAPHITEM_H
#define PATHFINDING_PROBLEM_GRAPHITEM_H

#include <string>

class GraphItem {
protected:
    unsigned long id;
public:
    virtual std::string get_info() const =0;
public:
    explicit GraphItem(unsigned long id): id(id){};

    friend class Graph;
};

#endif //PATHFINDING_PROBLEM_GRAPHITEM_H
