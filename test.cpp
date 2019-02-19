#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <random>
#include <ctime>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "classes/Edge/Edge.h"
#include "classes/Vertex/Vertex.h"
#include "classes/Graph/Graph.h"

#include "classes/Utilities/ShaderLoader/ShaderLoader.h"

//using  namespace glm;

int main(){
    clock_t start = clock();
    auto g = new Graph();
    std::string node_file("../maps/nodes/SPb3_nodes.csv"),
        road_file("../maps/roads/SPb3_roads.csv");
    if (int x = g->read_graph_from_files(node_file, road_file)){
        std::cout << x << std::endl;
        clock_t end = clock();
        //g->get_info();
        printf("\n%ld\n%ld", end - start , CLOCKS_PER_SEC);
        delete g;
    }
    return 0;
}

