cmake_minimum_required(VERSION 3.12)
project(Pathfinding_Problem)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCE_EXE main.cpp)

# Graph components
set(VERTEX_CPP classes/Vertex/Vertex.cpp)
set(EDGE_CPP classes/Edge/Edge.cpp)
set(GRAPH_CPP classes/Graph/Graph.cpp)

# Utils
set(CSVUTIL_CPP classes/Utilities/CSVUtil/CSVUtil.cpp)
set(SHADERLOADER_CPP classes/Utilities/ShaderLoader/ShaderLoader.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_EXE}
        ${VERTEX_CPP}
        ${EDGE_CPP}
        ${GRAPH_CPP}
        ${CSVUTIL_CPP}
        ${SHADERLOADER_CPP} gui/MainWindow.cpp gui/CustomOpenGLWidget.cpp
        gui/VertexButton.cpp
        gui/custom_widgets/MultiToggleButton.cpp
        gui/custom_widgets/MultiToggleButton.h
        gui/custom_widgets/ColorSelectButton.h
        gui/custom_widgets/ColorChooser.cpp
        gui/custom_widgets/ColorChooser.h
        gui/custom_widgets/MultiToggleButton.cpp
        gui/custom_widgets/MultiToggleButton.h
        gui/custom_widgets/MapInfo.cpp
        gui/custom_widgets/MapInfo.h
        gui/custom_widgets/ColorSelectButton.h
        classes/Category.cpp
        classes/Category.h
        classes/Algorithms.cpp
        classes/Algorithms.h
        gui/custom_widgets/StartSearchButton.h
        gui/DefaultGuiSettings.cpp
        gui/AlgThread.cpp
        gui/AlgThread.h)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
#find_package(glfw3 REQUIRED)
#find_package(glm REQUIRED)

find_package(Qt5Widgets CONFIG REQUIRED)

message(" ")

if (OPENGL_FOUND)
    message("opengl found")
    message("include dir: ${OPENGL_INCLUDE_DIR}")
    message("link libraries: ${OPENGL_LIBRARY}")
    message(" ")
endif()

#if (GLUT_FOUND)
#    message("GLUT found")
#    message("include dir: ${GLUT_INCLUDE_DIR}")
#    message("link libraries: ${GLUT_LIBRARY}")
#    message(" ")
#endif()

#if (GLEW_FOUND)
#    message("FLEW found")
#    message("include dir: ${GLEW_INCLUDE_DIR}")
#    message("link libraries: ${GLEW_LIBRARY}")
#    message(" ")
#endif()
#
#if (glfw3_FOUND)
#    message("glfw3 found")
#    message(" ")
#endif()
#
#if (glm_FOUND)
#    message("glm found")
#    message(" ")
#endif()

if (Qt5Widgets_FOUND)
    message(Qt5 found)
    message(" ")
endif()

include_directories(${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARY}
        ${OPENGL_gl_LIBRARY}
#        ${GLUT_LIBRARY}
        ${GLEW_LIBRARY}
#        glfw
        Qt5::Widgets)
