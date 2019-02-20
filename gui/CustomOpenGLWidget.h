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
#include "../classes/Graph/Graph.h"

class CustomOpenGLWidget : public QOpenGLWidget
{
public:
    CustomOpenGLWidget(QWidget *parent = 0);
    ~CustomOpenGLWidget();
    void setGraph(Graph* g);

protected:
    Graph* graph;
    QOpenGLShaderProgram* shaderProgram;
    float zoom , shiftX, shiftY;
    float zoomAngle;
    bool wasMousePressed;
    float recentShiftX, recentShiftY;
    //QOpenGLFunctions *f;

    QOpenGLShaderProgram* load_shaders(std::string v, std::string f);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mouseDoubleClickEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;
};


#endif //PATHFINDING_PROBLEM_CUSTOMOPENGLWIDGET_H
