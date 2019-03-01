
#include "../classes/Edge/Edge.h"
#include "../classes/Vertex/Vertex.h"

#include <QWidget>
#include <QOpenGLFunctions>
#include "CustomOpenGLWidget.h"
#include <cmath>
#include <QWheelEvent>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <QPushButton>

CustomOpenGLWidget::CustomOpenGLWidget(QWidget *parent)
        : QOpenGLWidget(parent), zoom(1), shiftX(0), shiftY(0), graph(nullptr),
          recentShiftX(0), recentShiftY(0),
          zoomAngle(0)
{
    setFocusPolicy(Qt::StrongFocus);

    //Setting MultiToggleButton
    QVector<QString> names = {"Select vertices",
                              "Create vertices",
                              "Select edges"};
    mtb = new MultiToggleButton(names, this);
    connect(mtb, SIGNAL(valueChanged(uint)), this, SLOT(changeClickMode(uint)));
    connect(this, SIGNAL(clickModeChangedByKey(uint)), mtb, SLOT(changeValue(uint)));
    clickMode = mtb->getState() + 1;

    //Setting MapInfo
    info = new MapInfo(this);
    connect(this, SIGNAL(amountsChanged(unsigned long, unsigned long, unsigned long,unsigned long)),
                info, SLOT(updateValues(unsigned long, unsigned long, unsigned long,unsigned long)));

    //Setting Vertex Size Slider
    vertexSizeSlider = new QSlider(this);
    vertexSizeSlider->setMinimum(1000);
    vertexSizeSlider->setMaximum(10000);
    vertexSizeSlider->setValue(vertexRadius * 1000);
    vertexSizeSlider->setMinimumHeight(200);
    vertexSizeSlider->setMinimumWidth(20);
    vertexSizeSlider->setVisible(false);
    connect(vertexSizeSlider, SIGNAL(valueChanged(int)),
            this, SLOT(changeVertexSize(int)));

}

void CustomOpenGLWidget::changeVertexSize(int value) {
    vertexRadius = value / 1000.0f;
    update();
}

void CustomOpenGLWidget::changeClickMode(uint mode) {
    clickMode = mode + 1;

}

CustomOpenGLWidget::~CustomOpenGLWidget()
{
    delete graph;
    delete [] preparedEdges;
    delete [] vertexTriangles;
    delete [] vtColors;
    delete graphShaderProgram;
    delete vertexShaderProgram;
    delete info;
    delete mtb;
    delete vertexSizeSlider;
}

void CustomOpenGLWidget::setGraph(Graph *g) {
    graph = g;
}

void CustomOpenGLWidget::initializeGL()
{
    emit amountsChanged(graph->vertices.size(),
                        graph->edges.size(),
                        selectedVertices.size(),
                        selectedEdges.size());
    mtb->setGeometry(width() - 400, height() - 40, 400, 40);
//    mtb->show();
//    info->show();
    vertexSizeSlider->setGeometry(width() - 20, 5, 0, 0);
    graphShaderProgram = load_shaders(
            "../shaders/edge_vertex_shader.glsl",
            "../shaders/edge_fragment_shader.glsl");
    vertexShaderProgram = load_shaders(
            "../shaders/vertex_vertex_shader.glsl",
            "../shaders/vertex_fragment_shader.glsl",
            "../shaders/vertex_geom_shader.glsl");
    prepareEdgesToDraw();
    prepareVertexToDraw();
    makeCurrent();
    initialWidth = width();
    initialHeight = height();
}

void CustomOpenGLWidget::paintGL()
{
    static int count = 0;
    //std::cout << "Repainting x" << count++ << "!\n";
    auto f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT);

    drawGraph(f);

    if (vertexHighlight) {
        highlightVertices(f);
    }

}

void CustomOpenGLWidget::resizeGL(int w, int h) {
    mtb->setGeometry(w - 400, h - 40, 400, 40);
    vertexSizeSlider->setGeometry(w - 20, 5, 0, 0);
}

void CustomOpenGLWidget::drawGraph(QOpenGLFunctions* f) const {

    if (!graphShaderProgram->bind()){
        throw std::runtime_error("Vertex shader isn't bound");
    }

    int minxyID = graphShaderProgram->uniformLocation("minxy");
    int maxxyID = graphShaderProgram->uniformLocation("maxxy");
    int zoomLocation = graphShaderProgram->uniformLocation("zoom");
    int posLocation = graphShaderProgram->attributeLocation("pos");
    int shiftLocation = graphShaderProgram->uniformLocation("shiftInPix");
    int screenRatioLocation = graphShaderProgram->uniformLocation("screenRatio");

    graphShaderProgram->setUniformValue(minxyID, (float)graph->minX, (float)graph->minY);
    graphShaderProgram->setUniformValue(maxxyID, (float)graph->maxX, (float)graph->maxY);
    graphShaderProgram->setUniformValue(zoomLocation, zoom);
    graphShaderProgram->setUniformValue(shiftLocation, shiftX, shiftY);
    graphShaderProgram->setUniformValue(screenRatioLocation, this->width(), this->height());
    graphShaderProgram->setUniformValue("baseRatio", initialWidth, initialHeight);

    graphShaderProgram->enableAttributeArray(posLocation);
    graphShaderProgram->setAttributeArray(posLocation, GL_FLOAT, preparedEdges, 2);
    f->glDrawArrays(GL_LINES, 0, 2 * (GLsizei)graph->edges.size());
}

void CustomOpenGLWidget::highlightVertices(QOpenGLFunctions *f) const {
    if (!vertexShaderProgram->bind()) {
        throw std::runtime_error("Vertex shader isn't bound");
    }

    int minxyID = vertexShaderProgram->uniformLocation("minxy");
    int maxxyID = vertexShaderProgram->uniformLocation("maxxy");
    int zoomLocation = vertexShaderProgram->uniformLocation("zoom");
    int posLocation = vertexShaderProgram->attributeLocation("pos");
    int shiftLocation = vertexShaderProgram->uniformLocation("shiftInPix");
    int screenRatioLocation = vertexShaderProgram->uniformLocation("screenRatio");
    int colorLocation = vertexShaderProgram->attributeLocation("color");


    vertexShaderProgram->setUniformValue(minxyID, (float) graph->minX, (float) graph->minY);
    vertexShaderProgram->setUniformValue(maxxyID, (float) graph->maxX, (float) graph->maxY);
    vertexShaderProgram->setUniformValue(zoomLocation, zoom);
    vertexShaderProgram->setUniformValue(shiftLocation, shiftX, shiftY);
    vertexShaderProgram->setUniformValue(screenRatioLocation, this->width(), this->height()/*initialWidth, initialHeight*/);
    vertexShaderProgram->setUniformValue("baseRatio", initialWidth, initialHeight);
    vertexShaderProgram->setUniformValue("radius", vertexRadius);

    vertexShaderProgram->enableAttributeArray(posLocation);

    vertexShaderProgram->setUniformValue("f", false);
    vertexShaderProgram->enableAttributeArray(posLocation);
    vertexShaderProgram->setAttributeArray(posLocation, vertexTriangles, 2, 2 * sizeof(float));
    vertexShaderProgram->enableAttributeArray(colorLocation);
    vertexShaderProgram->setAttributeArray(colorLocation, vtColors, 3, 3 * sizeof(float));
    f->glDrawArrays(GL_POINTS, 0, graph->vertices.size());
}

QOpenGLShaderProgram* CustomOpenGLWidget::load_shaders(const std::string& vs_path,
                                                        const std::string& fs_path,
                                                        const std::string& gs_path) {

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

    //GEOMETRY SHADER
    QOpenGLShader g_shader(QOpenGLShader::Geometry);
    if (gs_path.length() > 2) {
        QString geomShaderCode;
        std::ifstream gs_stream(gs_path, std::ios::in);
        if (gs_stream.is_open()) {
            std::stringstream sstr;
            sstr << gs_stream.rdbuf();
            geomShaderCode = sstr.str().c_str();
            gs_stream.close();
        } else {
            std::cout << gs_path << std::endl;
            throw std::runtime_error("Cannot open geometry shader code");
        }

        //compile vertex shader
        g_shader.compileSourceCode(geomShaderCode);

        //check shader
        if (!g_shader.isCompiled()) {
            std::cout << g_shader.log().toStdString() << std::endl;
        }
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
    auto program = new QOpenGLShaderProgram;
    program->addShader(&v_shader);
    program->addShader(&f_shader);
    if (gs_path.length() > 2){
        program->addShader(&g_shader);
    }

    program->link();
    if (!program->isLinked()){
        std::cout << program->log().toStdString() << std::endl;
        throw std::runtime_error("Program isn't linked");
    }

    return program;
}

void CustomOpenGLWidget::mouseDoubleClickEvent(QMouseEvent *e) {

}

void CustomOpenGLWidget::mousePressEvent(QMouseEvent *e) {
    wasMouseMoved = false;
    recentShiftX = e->pos().x();
    recentShiftY = e->pos().y();
    QVector2D v = convertCurrentPointFromMapToCoords(
            {(float)e->pos().x(), (float)e->pos().y()});
    printf("%f %f\n", v.x(), v.y());
}

void CustomOpenGLWidget::mouseReleaseEvent(QMouseEvent *e) {
    if (wasMouseMoved) return;
    std::cout << "Clicked. Current mode: " << clickMode << std::endl;
    if (clickMode == 1) {
        QVector2D coord = convertCurrentPointFromMapToCoords({(float) e->x(), (float) e->y()});
        Vertex *found = graph->getTheClosestVertex(coord.x(), coord.y(), 0.1);
        if (found != nullptr) {
            selectVertex(found);
            prepareVertexToDraw();
            update();
            emit amountsChanged(graph->vertices.size(),
                                graph->edges.size(),
                                selectedVertices.size(),
                                selectedEdges.size());

        }
        return;
    }

    if (clickMode == 2){
        QVector2D coord = convertCurrentPointFromMapToCoords({(float) e->x(), (float) e->y()});
        graph->addVertex(new Vertex(-1, coord.x(), coord.y()));
        emit amountsChanged(graph->vertices.size(),
                            graph->edges.size(),
                            selectedVertices.size(),
                            selectedEdges.size());
        prepareVertexToDraw();
        update();
        return;
    }

    if (clickMode == 3){
        return;
    }
}

void CustomOpenGLWidget::mouseMoveEvent(QMouseEvent *e) {
    wasMouseMoved = true;
    shiftX += (e->pos().x() - recentShiftX) / zoom;
    shiftY += (e->pos().y() - recentShiftY) / zoom;
    recentShiftX = e->pos().x();
    recentShiftY = e->pos().y();
    update();
}

void CustomOpenGLWidget::wheelEvent(QWheelEvent *e) {
    zoomAngle -= e->angleDelta().y();
    if (zoomAngle > 360.0f * 80) {
        zoomAngle = 360.f * 80;
        zoom = (float)pow(2, zoomAngle / 360);
    } else if (zoomAngle < -360.0f * 20) {
        zoomAngle = -360.0f * 20;
        zoom = (float)pow(2, zoomAngle / 360);
    } else {
        zoom = (float)pow(2, zoomAngle / 360);
        update();
    }
    /*std::cout << "Current zoom: " << zoom << std::endl;
    std::cout << "Current angle: " << zoomAngle << std::endl;*/
}

void CustomOpenGLWidget::keyPressEvent(QKeyEvent *e) {
    printf("%d = %s\n", e->key(), e->text().toStdString().c_str());
    int key = e->key();

    if (/* 1 */49 <= key && key <= 51/* 3 */){
        emit clickModeChangedByKey(uint(key) - 49);
        clickMode = uint(key) - 48;
    }

    //highlight vertices
    if (key == /* h */72 || key == /* р */1056){
        vertexSizeSlider->setVisible(!vertexSizeSlider->isVisible());
        vertexHighlight = !vertexHighlight;
        update();
        return;
    }

    //connect vertices
    int count = 0;
    if ((key == /* c */67 || key == /* c */1057) && selectedVertices.size() >= 2){
        for (auto i = selectedVertices.begin(); i != selectedVertices.end(); i++){
            for (auto j = i; j != selectedVertices.end(); j++){
                if (j == i) continue;
                count++;
                graph->addEdge(new Edge(i->second,
                        j->second, -1));
            }
        }
        std::cout << count << std::endl;
        selectedVertices.clear();
        prepareEdgesToDraw();
        prepareVertexToDraw();
        update();
        emit amountsChanged(graph->vertices.size(),
                            graph->edges.size(),
                            selectedVertices.size(),
                            selectedEdges.size());
        return;
    }

    //unselect (drop select)
    if (key == /* d */68 || key == /* в */1042){
        clearSelectedVertices();
        clearSelectedEdges();
        return;
    }
}

QVector2D CustomOpenGLWidget::convertPointFromCanvasToMap(const QVector2D &v) {
    QVector2D res;
    res.setX((v.x() + 1) / 2 * width());
    res.setY((-v.y() + 1) / 2 * height());
    return res;
}

QVector2D CustomOpenGLWidget::convertPointFromMapToCanvas(const QVector2D &v) {
    QVector2D res;
    res.setX(v.x() / width() * 2 - 1);
    res.setY(-v.y() / height() * 2 + 1);
    return res;
}

QVector2D CustomOpenGLWidget::convertFromCoordsToMap(const QVector2D &v) {
    float deltaX = graph->maxX - graph->minX;
    float deltaY = graph->maxY - graph->minY;

    QVector2D res;
    res.setX((v.x() - graph->minX) / deltaX * width());
    res.setY(height() - (v.y() - graph->minY) / deltaY * height());
    return res;
}

QVector2D CustomOpenGLWidget::convertFromMapToCoords(const QVector2D &v) {
    float deltaX = graph->maxX - graph->minX;
    float deltaY = graph->maxY - graph->minY;

    QVector2D res;
    res.setX(v.x() / width() * deltaX + graph->minX);
    res.setY((height() - v.y()) / height() * deltaY + graph->minY);
    return res;
}

QVector2D CustomOpenGLWidget::convertCurrentPointFromMapToCoords(const QVector2D &v) {
    float x, y;
    x = v.x() / width() * 2 - 1;
    x = ((x / zoom * width() + initialWidth) / 2 - shiftX) / initialWidth *
                (float)(graph->maxX - graph->minX) + (float)graph->minX;
    y = (height() - v.y()) / height() * 2 - 1;
    y = ((y / zoom * height() + initialHeight) / 2 + shiftY) / initialHeight *
        (float)(graph->maxY - graph->minY) + (float)graph->minY;
    return {x, y};
}

void CustomOpenGLWidget::prepareEdgesToDraw(){
    if (preparedEdges != nullptr) delete [] preparedEdges;
    preparedEdges = new float[4 * graph->edges.size()];
    int i = 0;
    for (auto pair = graph->edges.begin(); pair != graph->edges.end(); pair++, i += 4){
        preparedEdges[i] = (float) pair->second->vFrom->lon;
        preparedEdges[i + 1] = (float) pair->second->vFrom->lat;
        preparedEdges[i + 2] = (float) pair->second->vTo->lon;
        preparedEdges[i + 3] = (float) pair->second->vTo->lat;
    }
}

void CustomOpenGLWidget::prepareVertexToDraw() {
    if (vertexTriangles != nullptr) delete [] vertexTriangles;
    if (vtColors != nullptr) delete [] vtColors;

    vertexTriangles = new float[graph->vertices.size() * 2]{0};
    int i = 0;
    for (auto v : graph->vertices){
        vertexTriangles[i] = (float)v.second->lon;
        vertexTriangles[i + 1] = (float)v.second->lat;
        i += 2;
    }
    vtColors = new float[graph->vertices.size() * 3];
    i = 0;
    for (auto v : graph->vertices){
        vtColors[i] = .99f;
        vtColors[i + 1] = .99f;
        vtColors[i + 2] = .99f;
        if (selectedVertices.find(v.second->id) != selectedVertices.end()) {
            vtColors[i] = .0f;
            vtColors[i + 1] = .7f;
        }
        i += 3;
    }
}

void CustomOpenGLWidget::clearSelectedVertices() {
    selectedVertices.clear();
    prepareVertexToDraw();
    update();
    emit amountsChanged(graph->vertices.size(),
                        graph->edges.size(),
                        selectedVertices.size(),
                        selectedEdges.size());
}

void CustomOpenGLWidget::clearSelectedEdges() {
    selectedEdges.clear();
    update();
    emit amountsChanged(graph->vertices.size(),
                        graph->edges.size(),
                        selectedVertices.size(),
                        selectedEdges.size());
}

bool CustomOpenGLWidget::selectVertex(Vertex *v) {
    std::cout << v->get_info() << std::endl;
    auto it = selectedVertices.find(v->id);
    if (it == selectedVertices.end()){
        selectedVertices.insert({v->id, v});
        return true;
    }
    selectedVertices.erase(it);
    return false;
}

