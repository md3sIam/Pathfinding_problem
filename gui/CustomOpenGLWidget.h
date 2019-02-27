//
// Created by md3s on 04.02.19.
//

#ifndef PATHFINDING_PROBLEM_CUSTOMOPENGLWIDGET_H
#define PATHFINDING_PROBLEM_CUSTOMOPENGLWIDGET_H


#include <QWidget>
#include <QLabel>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <gui/VertexButton.h>
#include "../classes/Graph/Graph.h"

class CustomOpenGLWidget : public QOpenGLWidget
{
public:
    explicit CustomOpenGLWidget(QWidget *parent = 0);
    ~CustomOpenGLWidget() override;
    void setGraph(Graph* g);

protected:
    QLabel* infoLabel;

    Graph* graph;
    QOpenGLShaderProgram* graphShaderProgram;
    QOpenGLShaderProgram* vertexShaderProgram;
    int initialWidth, initialHeight;
    float vertexRadius = 6;
    float zoom,
        shiftX,
        shiftY;
    float zoomAngle;
    float recentShiftX,
        recentShiftY;
    float* preparedEdges = nullptr;
    float* vertexTriangles = nullptr;
    float* vtColors = nullptr;
    bool wasMouseMoved;
    bool vertexHighlight = false;
    std::vector<Vertex*> selectedVertices;


    QOpenGLShaderProgram* load_shaders(const std::string& v, const std::string& f, const std::string& g = "");
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    //MOUSE EVENTS
    void mouseDoubleClickEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;
    //KEY EVENTS
    void keyPressEvent(QKeyEvent* e) override;

private:
    void drawGraph(QOpenGLFunctions* f) const;
    void highlightVertices(QOpenGLFunctions* f) const;

    void prepareEdgesToDraw();
    void prepareVertexToDraw();

    QVector2D convertPointFromMapToCanvas(const QVector2D &v);
    QVector2D convertPointFromCanvasToMap(const QVector2D &v);
    QVector2D convertFromCoordsToMap(const QVector2D& v);
    QVector2D convertFromMapToCoords(const QVector2D& v);

    QVector2D convertCurrentPointFromMapToCoords(const QVector2D& v);
    QVector2D convertCurrentPointFromCoordsToMap(const QVector2D& v);
};


#endif //PATHFINDING_PROBLEM_CUSTOMOPENGLWIDGET_H
