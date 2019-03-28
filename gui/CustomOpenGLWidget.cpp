
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

    // Setting up inner signals and slot
    connect(this, SIGNAL(highlightSig(bool)), this, SLOT(highlightSl(bool)));

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
    restoreDefaultView();
    if (graph != nullptr) delete graph;
    selectedVertices.clear();
    selectedVerticesOrder.clear();
    selectedEdges.clear();
    graph = g;
    std::cout << graph->vertices.size() << '\n';
    std::cout << graph->edges.size() << '\n';
    prepareEdgesToDraw();
    prepareVertexToDraw();
    update();
    emit amountsChanged(graph->vertices.size(),
                        graph->edges.size(),
                        selectedVertices.size(),
                        selectedEdges.size());
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

    drawEdges(f);

    if (vertexHighlight) {
        highlightVertices(f);
    }

}

void CustomOpenGLWidget::resizeGL(int w, int h) {
    mtb->setGeometry(w - 400, h - 40, 400, 40);
    vertexSizeSlider->setGeometry(w - 20, 5, 0, 0);
}

void CustomOpenGLWidget::drawEdges(QOpenGLFunctions* f) const {

    if (!graphShaderProgram->bind()){
        throw std::runtime_error("Vertex shader isn't bound");
    }

    int minxyID = graphShaderProgram->uniformLocation("minxy");
    int maxxyID = graphShaderProgram->uniformLocation("maxxy");
    int zoomLocation = graphShaderProgram->uniformLocation("zoom");
    int posLocation = graphShaderProgram->attributeLocation("pos");
    int colorLocation = graphShaderProgram->attributeLocation("color");
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
    graphShaderProgram->enableAttributeArray(colorLocation);
    graphShaderProgram->setAttributeArray(colorLocation, GL_FLOAT, edgesColors, 3, 3 * sizeof(float));
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
//    vertexShaderProgram->disableAttributeArray(posLocation);
//    vertexShaderProgram->disableAttributeArray(colorLocation);
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
        QVector2D coord = convertCurrentPointFromMapToCoords({(float) e->x(), (float) e->y()});
        Edge* found = graph->getTheClosestEdge(coord.x(), coord.y(), 0.1);
        if (found != nullptr){
            selectEdge(found);
            prepareEdgesToDraw();
            update();
            emit amountsChanged(graph->vertices.size(),
                                graph->edges.size(),
                                selectedVertices.size(),
                                selectedEdges.size());
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
    if ((key == Qt::Key_C || key == /* c */1057) && selectedVertices.size() >= 2){
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
    std::cout << d << std::endl;
    std::cout << width() << std::endl;
    std::cout << height() << std::endl;
    std::cout << (graph->maxX - graph->minX) << std::endl;
    std::cout << (graph->maxY - graph->minY) << std::endl;
    std::cout << x << '\n' << y << std::endl;
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

void CustomOpenGLWidget::prepareEdgesToDraw(){
    if (preparedEdges != nullptr) delete [] preparedEdges;
    if (edgesColors != nullptr) delete [] edgesColors;

    //Setting coordinates and colors of not selected
    preparedEdges = new float[10 * graph->edges.size()];
    edgesColors = new float[6 * graph->edges.size()];
    int i = 0;
    int j = 0;

    float r = (float)edgeColor.red() / 255.0f;
    float g = (float)edgeColor.green() / 255.0f;
    float b = (float)edgeColor.blue() / 255.0f;

    for (auto pair : graph->edges){
        if (selectedEdges.find(pair.second->id) == selectedEdges.end()) {
            preparedEdges[i] = (float) pair.second->vFrom->lon;
            preparedEdges[i + 1] = (float) pair.second->vFrom->lat;
            preparedEdges[i + 2] = (float) pair.second->vTo->lon;
            preparedEdges[i + 3] = (float) pair.second->vTo->lat;
            edgesColors[j] = r;
            edgesColors[j + 1] = g;
            edgesColors[j + 2] = b;
            edgesColors[j + 3] = r;
            edgesColors[j + 4] = g;
            edgesColors[j + 5] = b;
            i += 4;
            j += 6;
        }
    }

    float sr = (float)selectedEdgesColor.red() / 255.0f;
    float sg = (float)selectedEdgesColor.green() / 255.0f;
    float sb = (float)selectedEdgesColor.blue() / 255.0f;

    //Setting coordinates and colors of selected
    for (auto pair : selectedEdges){
        preparedEdges[i] = (float) pair.second->vFrom->lon;
        preparedEdges[i + 1] = (float) pair.second->vFrom->lat;
        preparedEdges[i + 2] = (float) pair.second->vTo->lon;
        preparedEdges[i + 3] = (float) pair.second->vTo->lat;
        edgesColors[j] = sr;
        edgesColors[j + 1] = sg;
        edgesColors[j + 2] = sb;
        edgesColors[j + 3] = sr;
        edgesColors[j + 4] = sg;
        edgesColors[j + 5] = sb;
        i += 4;
        j += 6;
    }
}

void CustomOpenGLWidget::prepareVertexToDraw() {
    if (vertexTriangles != nullptr) delete [] vertexTriangles;
    if (vtColors != nullptr) delete [] vtColors;

    //Setting coordinates and colors of not selected
    vertexTriangles = new float[graph->vertices.size() * 2];
    vtColors = new float[graph->vertices.size() * 3];
    int i = 0;
    int j = 0;

    float r = (float)vertexColor.red() / 255.0f;
    float g = (float)vertexColor.green() / 255.0f;
    float b = (float)vertexColor.blue() / 255.0f;

    for (auto v : graph->vertices){
        if (selectedVertices.find(v.second->id) == selectedVertices.end()) {
            vertexTriangles[i] = (float) v.second->lon;
            vertexTriangles[i + 1] = (float) v.second->lat;
            i += 2;

            vtColors[j] = r;
            vtColors[j + 1] = g;
            vtColors[j + 2] = b;
            j += 3;
        }
    }
    //Setting coordinates and colors of selected
    float sr = (float)selectedVertexColor.red() / 255.0f;
    float sg = (float)selectedVertexColor.green() / 255.0f;
    float sb = (float)selectedVertexColor.blue() / 255.0f;
    for (auto v : selectedVertices){
        vertexTriangles[i] = (float) v.second->lon;
        vertexTriangles[i + 1] = (float) v.second->lat;
        i += 2;

        vtColors[j] = sr;
        vtColors[j + 1] = sg;
        vtColors[j + 2] = sb;
        j += 3;
    }
}

void CustomOpenGLWidget::clearSelectedVertices() {
    selectedVertices.clear();
    selectedVerticesOrder.clear();
    prepareVertexToDraw();
    update();
    emit amountsChanged(graph->vertices.size(),
                        graph->edges.size(),
                        selectedVertices.size(),
                        selectedEdges.size());
}

void CustomOpenGLWidget::clearSelectedEdges() {
    selectedEdges.clear();
    prepareEdgesToDraw();
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
        selectedVerticesOrder.push_back(v);
        return true;
    }
    selectedVertices.erase(it);
    selectedVerticesOrder.remove(v);
    return false;
}

bool CustomOpenGLWidget::selectEdge(Edge *e){
    std::cout << e->get_info() << std::endl;
    auto it = selectedEdges.find(e->id);
    if (it == selectedEdges.end()){
        selectedEdges.insert({e->id, e});
        return true;
    }
    selectedEdges.erase(it);
    return false;
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
    for (auto i = selectedVertices.begin(); i != selectedVertices.end(); i++){
        for (auto j = i; j != selectedVertices.end(); j++){
            if (j == i) continue;
            graph->addEdge(new Edge(i->second,
                                    j->second, -1));
        }
    }
    prepareEdgesToDraw();
    prepareVertexToDraw();
    update();
    emit amountsChanged(graph->vertices.size(),
                        graph->edges.size(),
                        selectedVertices.size(),
                        selectedEdges.size());
}

void CustomOpenGLWidget::linkSelVerticesTogether() {
    linkAllSelectedVerticesTogether();
}

void CustomOpenGLWidget::linkSelectedVerticesSequentially() {
    for (auto i = selectedVerticesOrder.begin(); i != selectedVerticesOrder.end();){
        auto prev = i++;
        if (i != selectedVerticesOrder.end())
            graph->addEdge(new Edge(*prev, *i, -1));
    }
    prepareEdgesToDraw();
    prepareVertexToDraw();
    update();
    emit amountsChanged(graph->vertices.size(),
                        graph->edges.size(),
                        selectedVertices.size(),
                        selectedEdges.size());
}

void CustomOpenGLWidget::linkSelVerticesSequentially() {
    linkSelectedVerticesSequentially();
}

void CustomOpenGLWidget::removeSelectedVertices() {
    for (auto pair : selectedVertices){
        graph->removeVertex(pair.second);
    }
    selectedVertices.clear();
    selectedVerticesOrder.clear();
    prepareEdgesToDraw();
    prepareVertexToDraw();
    update();
    emit amountsChanged(graph->vertices.size(),
                        graph->edges.size(),
                        selectedVertices.size(),
                        selectedEdges.size());
}

void CustomOpenGLWidget::removeSelectedEdges() {
    for (auto pair : selectedEdges){
        graph->removeEdge(pair.second);
    }
    selectedEdges.clear();
    prepareEdgesToDraw();
    prepareVertexToDraw();
    update();
    emit amountsChanged(graph->vertices.size(),
                        graph->edges.size(),
                        selectedVertices.size(),
                        selectedEdges.size());
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
    prepareVertexToDraw();
    update();
}

void CustomOpenGLWidget::changeSelectedVertexColor(const QColor& color) {
    selectedVertexColor = color;
    prepareVertexToDraw();
    update();
}

void CustomOpenGLWidget::changeEdgeColor(const QColor& color) {
    edgeColor = color;
    prepareEdgesToDraw();
    update();
}

void CustomOpenGLWidget::changeSelectedEdgeColor(const QColor& color) {
    selectedEdgesColor = color;
    prepareEdgesToDraw();
    update();
}