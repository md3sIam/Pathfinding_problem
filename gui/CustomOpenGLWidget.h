//
// Created by md3s on 04.02.19.
//

#ifndef PATHFINDING_PROBLEM_CUSTOMOPENGLWIDGET_H
#define PATHFINDING_PROBLEM_CUSTOMOPENGLWIDGET_H

#include <map>
#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <gui/VertexButton.h>
#include "../classes/Graph/Graph.h"
#include "custom_widgets/MultiToggleButton.h"
#include "custom_widgets/MapInfo.h"

class CustomOpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit CustomOpenGLWidget(QWidget *parent = 0);
    ~CustomOpenGLWidget() override;
    void setGraph(Graph* g);

public slots:
    void changeVertexSize(int value);
    void changeClickMode(uint mode);

signals:
    void amountsChanged(unsigned long, unsigned long, unsigned long,unsigned long);
    void clickModeChangedByKey(uint);

protected:
    //INTERFACE
    MapInfo* info;
    MultiToggleButton* mtb;
    QSlider* vertexSizeSlider;
    uint clickMode;

    //OPENGL INFO
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
    std::map<long, Vertex*> selectedVertices;
    std::map<unsigned long, Edge*> selectedEdges;


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

//    void resizeEvent(QResizeEvent* e) override;

private:
    void drawGraph(QOpenGLFunctions* f) const;
    void highlightVertices(QOpenGLFunctions* f) const;

    void prepareEdgesToDraw();
    void prepareVertexToDraw();

    void clearSelectedVertices();
    void clearSelectedEdges();

    bool selectVertex(Vertex* v);

    QVector2D convertPointFromMapToCanvas(const QVector2D &v);
    QVector2D convertPointFromCanvasToMap(const QVector2D &v);
    QVector2D convertFromCoordsToMap(const QVector2D& v);
    QVector2D convertFromMapToCoords(const QVector2D& v);

    QVector2D convertCurrentPointFromMapToCoords(const QVector2D& v);
    QVector2D convertCurrentPointFromCoordsToMap(const QVector2D& v);
};


#endif //PATHFINDING_PROBLEM_CUSTOMOPENGLWIDGET_H
