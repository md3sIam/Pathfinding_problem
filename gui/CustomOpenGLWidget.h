//
// Created by md3s on 04.02.19.
//

#ifndef PATHFINDING_PROBLEM_CUSTOMOPENGLWIDGET_H
#define PATHFINDING_PROBLEM_CUSTOMOPENGLWIDGET_H


#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <gui/VertexButton.h>
#include "../classes/Graph/Graph.h"

class CustomOpenGLWidget : public QOpenGLWidget
{
public:
    CustomOpenGLWidget(QWidget *parent = 0);
    ~CustomOpenGLWidget();
    void setGraph(Graph* g);

protected:
    Graph* graph;
    QOpenGLShaderProgram* graphShaderProgram;
    QOpenGLShaderProgram* vertexShaderProgram;
    float vertexSizeX = 1;
    float vertexSizeY = 1;
    float zoom,
        shiftX,
        shiftY;
    float zoomAngle;
    bool wasMousePressed;
    float recentShiftX,
        recentShiftY;
//    std::vector<VertexButton*> vertexButtons;
    float* preparedEdges;
    float* vertexTriangles;
    float* vtColors;


    QOpenGLShaderProgram* load_shaders(const std::string& v, const std::string& f, const std::string& g = "");
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mouseDoubleClickEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;

private:
    void prepareEdgesToDraw();
    void prepareVertexTrianglesToDraw();
    void prepareVertexToDraw();

    QVector2D convertPointFromMapToCanvas(const QVector2D &v);
    QVector2D convertPointFromCanvasToMap(const QVector2D &v);
    QVector2D convertFromCoordsToMap(const QVector2D& v);
    QVector2D convertFromMapToCoords(const QVector2D& v);

};


#endif //PATHFINDING_PROBLEM_CUSTOMOPENGLWIDGET_H
