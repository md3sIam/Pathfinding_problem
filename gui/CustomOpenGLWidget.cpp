
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
#include <QPushButton>

CustomOpenGLWidget::CustomOpenGLWidget(QWidget *parent)
        : QOpenGLWidget(parent), zoom(1), shiftX(0), shiftY(0), graph(nullptr),
          recentShiftX(0), recentShiftY(0),
          zoomAngle(0)
{
    // Setting up parameters of widget
    setFocusPolicy(Qt::StrongFocus);

    vertexColor = QColor(255, 255, 255);
    selectedVertexColor = QColor(0, 78, 255);

    edgeColor = QColor(255, 255, 255);
    selectedEdgesColor = QColor(0, 128, 255);

    initVertexHandler();
    initEdgeHandler();

    // Setting up inner signals and slot
    connect(this, SIGNAL(highlightSig(bool)), this, SLOT(highlightSl(bool)));
    connect(this, SIGNAL(amountsChanged(unsigned long, unsigned long, unsigned long,unsigned long)),
            this, SLOT(checkForEnablingSearchButton(unsigned long, unsigned long, unsigned long,unsigned long)));

    // Setting MultiToggleButton
    QVector<QString> names = {"Select vertices",
                              "Create vertices",
                              "Select edges"};
    mtb = new MultiToggleButton(names, this);
    mtb->setVisible(false);
    connect(mtb, SIGNAL(valueChanged(uint)), this, SLOT(changeClickMode(uint)));
    connect(this, SIGNAL(clickModeChangedByKey(uint)), mtb, SLOT(changeValue(uint)));
    clickMode = mtb->getState() + 1;

    //Setting MapInfo
    info = new MapInfo(this);
    info->move(10, 10);
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

void CustomOpenGLWidget::highlightSl(bool h) {
    vertexHighlight = h;
    mtb->setVisible(h);
    vertexSizeSlider->setVisible(h);
    update();
}

CustomOpenGLWidget::~CustomOpenGLWidget()
{
    delete graph;

    delete edge_default_shader_program;
    delete vertex_default_shader_program;
    delete info;
    delete mtb;
    delete vertexSizeSlider;
}

void CustomOpenGLWidget::setGraph(Graph *g) {
    restoreDefaultView();
    delete graph;
    vertexHandler["Selected"].clear();
    edgeHandler["Selected"].clear();
    graph = g;
    for (std::pair<const unsigned long, Vertex*>& pair : graph->vertices)
        vertexHandler.addToType("Core", pair.second);
    for (std::pair<const unsigned long, Edge*>& pair : graph->edges)
        edgeHandler.addToType("Core", pair.second);
//    prepareAllEdgesToDraw();
    //prepareAllVerticesToDraw();
    update();
    emit amountsChanged(graph->vertices.size(),
                        graph->edges.size(),
                        vertexHandler["Selected"].getSize()/*selectedVertices.size()*/,
                        edgeHandler["Selected"].getSize());
}

void CustomOpenGLWidget::initVertexHandler() {
    vertexHandler.createType("Core", vertexColor);
    vertexHandler.createType("Selected", selectedVertexColor);
}

void CustomOpenGLWidget::initEdgeHandler() {
    edgeHandler.createType("Core", edgeColor);
    edgeHandler.createType("Selected", selectedEdgesColor);
}

void CustomOpenGLWidget::initializeGL()
{
    emit amountsChanged(graph->vertices.size(),
                        graph->edges.size(),
                        vertexHandler["Selected"].getSize()/*selectedVertices.size()*/,
                        edgeHandler["Selected"].getSize());
    mtb->setGeometry(width() - 400, height() - 40, 400, 40);
    vertexSizeSlider->setGeometry(width() - 20, 5, 0, 0);
    edge_default_shader_program = load_shaders(
            "../shaders/v2/edges_vertex_const_color_shader.glsl",
            "../shaders/v2/edges_fragment_const_color_shader.glsl"
            );
    vertex_default_shader_program = load_shaders(
            "../shaders/v2/vertices_vertex_const_color_shader.glsl",
            "../shaders/v2/vertices_fragment_const_color_shader.glsl",
            "../shaders/v2/vertices_geom_const_color_shader.glsl");
//    prepareAllEdgesToDraw();
    //prepareAllVerticesToDraw();

    makeCurrent();
    initialWidth = width();
    initialHeight = height();
}

void CustomOpenGLWidget::paintGL()
{
    auto f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT);

    drawAllEdges();

    if (vertexHighlight) {
        drawAllVertices();
    }

}

void CustomOpenGLWidget::resizeGL(int w, int h) {
    mtb->setGeometry(w - 400, h - 40, 400, 40);
    vertexSizeSlider->setGeometry(w - 20, 5, 0, 0);
}

/*void CustomOpenGLWidget::drawCoreEdges() {
    drawEdges(edge_default_shader_program,
            edgeColor,
            preparedCoreEdges,
            2 * (graph->edges.size() - selectedEdges.size()));
}

void CustomOpenGLWidget::drawSelectedEdges() {
    drawEdges(edge_default_shader_program,
            selectedEdgesColor,
            preparedSelectedEdges,
            2 * selectedEdges.size());
}*/

void CustomOpenGLWidget::drawAllEdges() {
    edgeHandler.drawType("Core", this);
    edgeHandler.drawType("Selected", this);
}

void CustomOpenGLWidget::drawEdges(QOpenGLShaderProgram* program,
                                const QColor& color,
                                const float* edges,
                                int size) const {
    auto f = QOpenGLContext::currentContext()->functions();

    if (!program->bind()){
        throw std::runtime_error("Vertex shader isn't bound");
    }

    int minxyID = program->uniformLocation("minxy");
    int maxxyID = program->uniformLocation("maxxy");
    int zoomLocation = program->uniformLocation("zoom");
    int posLocation = program->attributeLocation("pos");
    int shiftLocation = program->uniformLocation("shiftInPix");
    int screenRatioLocation = program->uniformLocation("screenRatio");

    program->setUniformValue(minxyID, (float)graph->minX, (float)graph->minY);
    program->setUniformValue(maxxyID, (float)graph->maxX, (float)graph->maxY);
    program->setUniformValue(zoomLocation, zoom);
    program->setUniformValue(shiftLocation, shiftX, shiftY);
    program->setUniformValue(screenRatioLocation, this->width(), this->height());
    program->setUniformValue("baseRatio", initialWidth, initialHeight);
    program->setUniformValue("color", (float)color.red() / 255.0f,
                             (float)color.green() / 255.0f,
                             (float)color.blue() / 255.0f);

    program->enableAttributeArray(posLocation);
    program->setAttributeArray(posLocation, GL_FLOAT, edges, 2);
    f->glDrawArrays(GL_LINES, 0, size);
}

/*void CustomOpenGLWidget::drawCoreVertices() {
    drawVertices(vertex_default_shader_program,
            vertexColor,
            preparedCoreVertices,
            2 * (graph->vertices.size() - selectedVertices.size()));
}

void CustomOpenGLWidget::drawSelectedVertices() {
    drawVertices(vertex_default_shader_program,
            selectedVertexColor,
            preparedSelectedVertices,
            2 * selectedVertices.size());
}*/

void CustomOpenGLWidget::drawAllVertices() {
    /*drawCoreVertices();
    drawSelectedVertices();*/
    vertexHandler.drawType("Core", this);
    vertexHandler.drawType("Selected", this);
}

void CustomOpenGLWidget::drawVertices(QOpenGLShaderProgram* program,
                                      const QColor& color,
                                      const float* vertices,
                                      int size) const {
    auto f = QOpenGLContext::currentContext()->functions();

    if (!program->bind()) {
        throw std::runtime_error("Vertex shader isn't bound");
    }

    int minxyID = program->uniformLocation("minxy");
    int maxxyID = program->uniformLocation("maxxy");
    int zoomLocation = program->uniformLocation("zoom");
    int posLocation = program->attributeLocation("pos");
    int shiftLocation = program->uniformLocation("shiftInPix");
    int screenRatioLocation = program->uniformLocation("screenRatio");


    program->setUniformValue(minxyID, (float) graph->minX, (float) graph->minY);
    program->setUniformValue(maxxyID, (float) graph->maxX, (float) graph->maxY);
    program->setUniformValue(zoomLocation, zoom);
    program->setUniformValue(shiftLocation, shiftX, shiftY);
    program->setUniformValue(screenRatioLocation, this->width(), this->height()/*initialWidth, initialHeight*/);
    program->setUniformValue("baseRatio", initialWidth, initialHeight);
    program->setUniformValue("radius", vertexRadius);
    program->setUniformValue("color", (float)color.red() / 255.0f,
                             (float)color.green() / 255.0f,
                             (float)color.blue() / 255.0f);

    program->enableAttributeArray(posLocation);

    program->setUniformValue("f", false);
    program->enableAttributeArray(posLocation);
    program->setAttributeArray(posLocation, GL_FLOAT, vertices, 2, 2 * sizeof(float));
    f->glDrawArrays(GL_POINTS, 0, size);
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
    if (wasMouseMoved || !vertexHighlight) return;
    //std::cout << "Clicked. Current mode: " << clickMode << std::endl;
    if (clickMode == 1) {
        QVector2D coord = convertCurrentPointFromMapToCoords({(float) e->x(), (float) e->y()});
        float radius = convertDistFromPixToCoords(pixelRadiusClickAreaSearch);
        Vertex *found = graph->getTheClosestVertex(coord.x(), coord.y(), radius);
        if (found != nullptr) {
            selectVertex(found);
            //prepareAllVerticesToDraw();
            update();
            emit amountsChanged(graph->vertices.size(),
                                graph->edges.size(),
                                vertexHandler["Selected"].getSize()/*selectedVertices.size()*/,
                                edgeHandler["Selected"].getSize());

        }
        return;
    }

    if (clickMode == 2){
        QVector2D coord = convertCurrentPointFromMapToCoords({(float) e->x(), (float) e->y()});
        auto v = new Vertex(-1, coord.x(), coord.y());
        graph->addVertex(v);
        vertexHandler.addToType("Core", v);
        selectVertex(v);
        emit amountsChanged(graph->vertices.size(),
                            graph->edges.size(),
                            vertexHandler["Selected"].getSize()/*selectedVertices.size()*/,
                            edgeHandler["Selected"].getSize());
        //prepareAllVerticesToDraw();
        update();
        return;
    }

    if (clickMode == 3){
        QVector2D coord = convertCurrentPointFromMapToCoords({(float) e->x(), (float) e->y()});
        Edge* found = graph->getTheClosestEdge(coord.x(), coord.y(), 0.1);
        if (found != nullptr){
            selectEdge(found);
            update();
            emit amountsChanged(graph->vertices.size(),
                                graph->edges.size(),
                                vertexHandler["Selected"].getSize()/*selectedVertices.size()*/,
                                edgeHandler["Selected"].getSize());
        }
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
}

void CustomOpenGLWidget::keyPressEvent(QKeyEvent *e) {
    //printf("%d = %s\n", e->key(), e->text().toStdString().c_str());
    int key = e->key();

    if (/*arrow up*/key == 16777235){
        shiftY += arrowShiftSpeed / zoom;
        update();
        return;
    }

    if (/*arrow down*/key == 16777237){
        shiftY -= arrowShiftSpeed / zoom;
        update();
        return;
    }

    if (/*arrow right*/key == 16777236){
        shiftX -= arrowShiftSpeed / zoom;
        update();
        return;
    }

    if (/*arrow left*/key == 16777234){
            shiftX += arrowShiftSpeed / zoom;
        update();
        return;
    }

    if (/* 1 */Qt::Key_1 <= key && key <= Qt::Key_3/* 3 */){
        emit clickModeChangedByKey(uint(key - Qt::Key_1));
        clickMode = uint(key - Qt::Key_1 + 1);
        return;
    }

    //highlight vertices
    if (key == Qt::Key_H || key == /* р */1056){
        emit highlightSig(!vertexHighlight);
        update();
        return;
    }

    //connect vertices
    int count = 0;
    if ((key == Qt::Key_C || key == /* c */1057) && vertexHandler["Selected"].getSize() >= 2){
        linkAllSelectedVerticesTogether();
        return;
    }

    //unselect (drop select)
    if (key == Qt::Key_D || key == /* в */1042){
        clearSelectedVertices();
        clearSelectedEdges();
        update();
        return;
    }

    if (key == Qt::Key_R|| key == /* к */1050){
        removeSelEdgesAndVertices();
        return;
    }

    if (key == Qt::Key_S && e->modifiers().testFlag(Qt::ControlModifier)){
        emit save();
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

float CustomOpenGLWidget::convertDistFromPixToCoords(int d) {
    float x = float(d) / width() * (graph->maxX - graph->minX) / zoom;
    float y = float(d) / height() * (graph->maxY - graph->minY) / zoom;
    return x > y ? x : y;
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

void CustomOpenGLWidget::clearSelectedVertices() {
    vertexHandler["Selected"].clear();
    //prepareAllVerticesToDraw();
    update();
    emit amountsChanged(graph->vertices.size(),
                        graph->edges.size(),
                        vertexHandler["Selected"].getSize()/*selectedVertices.size()*/,
                        edgeHandler["Selected"].getSize());
}

void CustomOpenGLWidget::clearSelectedEdges() {
    edgeHandler["Selected"].clear();
    update();
    emit amountsChanged(graph->vertices.size(),
                        graph->edges.size(),
                        vertexHandler["Selected"].getSize()/*selectedVertices.size()*/,
                        edgeHandler["Selected"].getSize());
}

bool CustomOpenGLWidget::selectVertex(Vertex *v) {
    // its not necessary to change core
    if (vertexHandler.typeContains("Selected", v)){
        vertexHandler.removeFromType("Selected", v);
        return true;
    }
    else {
        vertexHandler.addToType("Selected", v);
        return false;
    }
}

bool CustomOpenGLWidget::selectEdge(Edge *e){

    if (edgeHandler.typeContains("Selected", e)){
        edgeHandler.removeFromType("Selected", e);
//        vertexHandler.addToType("Core", v);
        return true;
    }
    else {
//        vertexHandler.removeFromType("Core", v);
        edgeHandler.addToType("Selected", e);
        return false;
    }
}

void CustomOpenGLWidget::restoreDefaultView() {
    zoom = 1;
    shiftX = 0;
    shiftY = 0;
    zoomAngle = 0;
    recentShiftX = 0;
    recentShiftY = 0;
    initialWidth = width();
    initialHeight = height();
    update();
}

void CustomOpenGLWidget::linkAllSelectedVerticesTogether() {
    const std::vector<Vertex*>& seq = vertexHandler["Selected"].getSequence();
    for (Vertex* vi : seq){
        for (Vertex* vj : seq){
            if (vj == vi) continue;
            Edge* e = new Edge(vi, vj, -1);
            graph->addEdge(e);
            edgeHandler["Core"].add(e);
            edgeHandler["Selected"].add(e);
        }
    }
    update();
    emit amountsChanged(graph->vertices.size(),
                        graph->edges.size(),
                        vertexHandler["Selected"].getSize()/*selectedVertices.size()*/,
                        edgeHandler["Selected"].getSize());
}

void CustomOpenGLWidget::linkSelVerticesTogether() {
    linkAllSelectedVerticesTogether();
}

void CustomOpenGLWidget::linkSelectedVerticesSequentially() {
    const std::vector<Vertex*>& seq = vertexHandler["Selected"].getSequence();
    for (auto i = seq.begin(); i != seq.end();){
        auto prev = i++;
        if (i != seq.end()) {
            Edge *e = new Edge(*prev, *i, -1);
            graph->addEdge(e);
            edgeHandler["Core"].add(e);
            edgeHandler["Selected"].add(e);
        }
    }
    update();
    emit amountsChanged(graph->vertices.size(),
                        graph->edges.size(),
                        vertexHandler["Selected"].getSize()/*selectedVertices.size()*/,
                        edgeHandler["Selected"].getSize());
}

void CustomOpenGLWidget::linkSelVerticesSequentially() {
    linkSelectedVerticesSequentially();
}

void CustomOpenGLWidget::removeSelectedVertices() {
    for (auto v : vertexHandler["Selected"].getSequence()) {
        for (auto it : v->incidentEdges) {
            //Removing edges from vectors of incident vertices for given vertex
            edgeHandler.remove(it);
        }
    }
    for (Vertex* v : vertexHandler["Selected"].getSequence()){
        vertexHandler.removeFromType("Core", v);
        graph->removeVertex(v);
    }
    vertexHandler["Selected"].clear();
    update();
    emit amountsChanged(graph->vertices.size(),
                        graph->edges.size(),
                        vertexHandler["Selected"].getSize(),
                        edgeHandler["Selected"].getSize());
}

void CustomOpenGLWidget::removeSelectedEdges() {
    for (Edge* e : edgeHandler["Selected"].getSequence()){
        edgeHandler.removeFromType("Core", e);
        graph->removeEdge(e);
    }
    edgeHandler["Selected"].clear();
    update();
    emit amountsChanged(graph->vertices.size(),
                        graph->edges.size(),
                        vertexHandler["Selected"].getSize(),
                        edgeHandler["Selected"].getSize());
}

void CustomOpenGLWidget::removeSelVertices() {
    removeSelectedVertices();
}

void CustomOpenGLWidget::removeSelEdges() {
    removeSelectedEdges();
}

void CustomOpenGLWidget::removeSelEdgesAndVertices() {
    removeSelectedEdges();
    removeSelectedVertices();
}

void CustomOpenGLWidget::dropSelVertices(){
    clearSelectedVertices();
}

void CustomOpenGLWidget::dropSelEdges() {
    clearSelectedEdges();
}

void CustomOpenGLWidget::dropSelEdgesAndVertices() {
    clearSelectedEdges();
    clearSelectedVertices();
}

void CustomOpenGLWidget::changeVertexColor(const QColor& color) {
    vertexColor = color;
    vertexHandler["Core"].setColor(vertexColor);
    update();
}

void CustomOpenGLWidget::changeSelectedVertexColor(const QColor& color) {
    selectedVertexColor = color;
    vertexHandler["Selected"].setColor(selectedVertexColor);
    update();
}

void CustomOpenGLWidget::changeEdgeColor(const QColor& color) {
    edgeColor = color;
    edgeHandler["Core"].setColor(edgeColor);
    update();
}

void CustomOpenGLWidget::changeSelectedEdgeColor(const QColor& color) {
    selectedEdgesColor = color;
    edgeHandler["Selected"].setColor(selectedEdgesColor);
    update();
}

void CustomOpenGLWidget::checkForEnablingSearchButton(unsigned long, unsigned long, unsigned long selected_amount, unsigned long) {
    if (selected_amount == 2)
        emit enableToSearchPath(true);
    else
        emit enableToSearchPath(false);
}

/*void CustomOpenGLWidget::prepareCoreEdgesToDraw() {
    if (preparedCoreEdges != nullptr) delete [] preparedCoreEdges;

    //Setting coordinates and colors of not selected
    preparedCoreEdges = new float[4 * (graph->edges.size() - selectedEdges.size())];
    int i = 0;

    for (auto pair : graph->edges){
        if (selectedEdges.find(pair.second->id) == selectedEdges.end()) {
            preparedCoreEdges[i] = (float) pair.second->vFrom->lon;
            preparedCoreEdges[i + 1] = (float) pair.second->vFrom->lat;
            preparedCoreEdges[i + 2] = (float) pair.second->vTo->lon;
            preparedCoreEdges[i + 3] = (float) pair.second->vTo->lat;
            i += 4;
        }
    }
}

void CustomOpenGLWidget::prepareSelectedEdgesToDraw() {
    if (preparedSelectedEdges != nullptr) delete[] preparedSelectedEdges;
    //Setting coordinates and colors of selected
    preparedSelectedEdges = new float[4 * selectedEdges.size()];
    int i = 0;
    for (auto pair : selectedEdges){
        preparedSelectedEdges[i] = (float) pair.second->vFrom->lon;
        preparedSelectedEdges[i + 1] = (float) pair.second->vFrom->lat;
        preparedSelectedEdges[i + 2] = (float) pair.second->vTo->lon;
        preparedSelectedEdges[i + 3] = (float) pair.second->vTo->lat;
        i += 4;
    }
}*/

/*void CustomOpenGLWidget::prepareAllEdgesToDraw() {
    prepareCoreEdgesToDraw();
    prepareSelectedEdgesToDraw();
}*/


