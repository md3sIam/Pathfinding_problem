cmake_minimum_required(VERSION 3.12)
project(Pathfinding_Problem)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_EXE test.cpp)

# Graph components
set(VERTEX_CPP classes/Vertex/Vertex.cpp)
set(EDGE_CPP classes/Edge/Edge.cpp)
set(GRAPH_CPP classes/Graph/Graph.cpp)

# Utils
set(CSVUTIL_CPP classes/Utilities/CSVUtil/CSVUtil.cpp)

add_executable(Pathfindind_Problem ${SOURCE_EXE}
        ${VERTEX_CPP}
        ${EDGE_CPP}
        ${GRAPH_CPP}
        ${CSVUTIL_CPP})

