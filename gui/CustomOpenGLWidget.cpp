//
// Created by md3s on 04.02.19.
//

#include <GL/glut.h>
#include "../classes/Edge/Edge.h"
#include "../classes/Vertex/Vertex.h"

#include <QOpenGLFunctions>
#include "CustomOpenGLWidget.h"
#include <cmath>
#include <QWheelEvent>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <exception>

CustomOpenGLWidget::CustomOpenGLWidget(QWidget *parent)
        : QOpenGLWidget(parent), zoom(1), shiftX(0), shiftY(0), graph(nullptr),
          wasMousePressed(false), recentShiftX(0), recentShiftY(0),
          zoomAngle(0)
{

}

CustomOpenGLWidget::~CustomOpenGLWidget()
{
    if (!graph){
        delete graph;
        delete shaderProgram;
    }
}

void CustomOpenGLWidget::setGraph(Graph *g) {
    graph = g;
}

void CustomOpenGLWidget::initializeGL()
{
    shaderProgram = load_shaders(
            "../shaders/first_vertex.glsl",
            "../shaders/first_fragment.glsl");
}

void CustomOpenGLWidget::paintGL()
{
    static int count = 0;
    std::cout << "Repainting x" << count++ << "!\n";
    auto f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glLineWidth(0.1);

    int minxyID = shaderProgram->uniformLocation("minxy");
    int maxxyID = shaderProgram->uniformLocation("maxxy");
    int zoomLocation = shaderProgram->uniformLocation("zoom");
    int posLocation = shaderProgram->attributeLocation("pos");
    int shiftLocation = shaderProgram->uniformLocation("shiftInPix");
    int screenRatioLocation = shaderProgram->uniformLocation("screenRatio");

    shaderProgram->setUniformValue(minxyID, (float)graph->minX, (float)graph->minY);
    shaderProgram->setUniformValue(maxxyID, (float)graph->maxX, (float)graph->maxY);
    shaderProgram->setUniformValue(zoomLocation, zoom);
    shaderProgram->setUniformValue(shiftLocation, shiftX, shiftY);
    shaderProgram->setUniformValue(screenRatioLocation, this->width(), this->height());

    shaderProgram->enableAttributeArray(posLocation);
    GLfloat* edges = graph->getEdgesPreparedToDraw();
    shaderProgram->setAttributeArray(posLocation, graph->getEdgesPreparedToDraw(), 2);
    //f->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    f->glDrawArrays(GL_LINES, 0, 2 * (GLsizei)graph->edges.size());
    shaderProgram->disableAttributeArray(posLocation);
    delete edges;
}

void CustomOpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5,0,0,0,0,1,0);
}

QOpenGLShaderProgram* CustomOpenGLWidget::load_shaders(std::string vs_path, std::string fs_path) {
//    std::string vs_path("../shaders/first_vertex.glsl");
//    std::string fs_path("../shaders/first_fragment.glsl");

//    f = QOpenGLContext::currentContext()->functions();
//    GLuint VertexShaderID = f->glCreateShader(GL_VERTEX_SHADER);


    //VERTEX SHADER
    QString vertexShaderCode;
    std::ifstream vs_stream(vs_path, std::ios::in);
    if (vs_stream.is_open()){
        std::stringstream sstr;
        sstr << vs_stream.rdbuf();
        vertexShaderCode = sstr.str().c_str();
        vs_stream.close();
    } else {
        throw std::runtime_error("Cannot open vertex shader code");
    }

    QOpenGLShader v_shader(QOpenGLShader::Vertex);

    //compile vertex shader
    v_shader.compileSourceCode(vertexShaderCode);

    //check shader
    if (!v_shader.isCompiled()){
        std::cout << v_shader.log().toStdString() << std::endl;
    }

    //FRAGMENT SHADER
    QString fragmentShaderCode;
    std::ifstream fs_stream(fs_path, std::ios::in);
    if (fs_stream.is_open()){
        std::stringstream sstr;
        sstr << fs_stream.rdbuf();
        fragmentShaderCode = sstr.str().c_str();
    } else {
        throw std::runtime_error("Cannot open fragment shader source code");
    }

    QOpenGLShader f_shader(QOpenGLShader::Fragment);

    //compile shader
    f_shader.compileSourceCode(fragmentShaderCode);

    //check shader
    if (!f_shader.isCompiled()){
        std::cout << f_shader.log().toStdString() << std::endl;
    }

    //CREATE A PROGRAM
    shaderProgram = new QOpenGLShaderProgram;
    shaderProgram->addShader(&v_shader);
    shaderProgram->addShader(&f_shader);

    shaderProgram->link();
    if (!shaderProgram->isLinked()){
        std::cout << shaderProgram->log().toStdString() << std::endl;
    }

    if (!shaderProgram->bind()){
        throw std::exception();
    }

    return shaderProgram;
}

void CustomOpenGLWidget::mouseDoubleClickEvent(QMouseEvent *e) {
    zoom *= 2;
    std::cout << "Zoom changed\n";
    update();
}

void CustomOpenGLWidget::mousePressEvent(QMouseEvent *e) {
    recentShiftX = e->pos().x();
    recentShiftY = e->pos().y();
}

void CustomOpenGLWidget::mouseReleaseEvent(QMouseEvent *e) {
}

void CustomOpenGLWidget::mouseMoveEvent(QMouseEvent *e) {
    wasMousePressed = true;
    shiftX += (e->pos().x() - recentShiftX) / zoom;
    shiftY += (e->pos().y() - recentShiftY) / zoom;
    recentShiftX = e->pos().x();
    recentShiftY = e->pos().y();
    update();
}

void CustomOpenGLWidget::wheelEvent(QWheelEvent *e) {
    zoomAngle -= e->angleDelta().y();
    if (zoomAngle > 360.0f * 8) {
        zoomAngle = 360.f * 8;
        zoom = pow(2, zoomAngle / 360);
    } else if (zoomAngle < -360.0f * 2) {
        zoomAngle = -360.0f * 2;
        zoom = pow(2, zoomAngle / 360);
    } else {
        zoom = pow(2, zoomAngle / 360);
        update();
    }
    std::cout << "Current zoom: " << zoom << std::endl;
    std::cout << "Current angle: " << zoomAngle << std::endl;
}