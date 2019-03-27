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
    // Ui
    void changeVertexSize(int value);
    void changeClickMode(uint mode);
    void highlightSl(bool);

    // Linking vertices
    void linkSelVerticesTogether();
    void linkSelVerticesSequentially();

    // Removing vertices or\and edges
    void removeSelVertices();
    void removeSelEdges();
    void removeSelEdgesAndVertices();

    // Clearing vertices or\and edges
    void dropSelVertices();
    void dropSelEdges();
    void dropSelEdgesAndVertices();

signals:
    void save();
    void amountsChanged(unsigned long, unsigned long, unsigned long,unsigned long);
    void clickModeChangedByKey(uint);
    void highlightSig(bool);

private:
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
    float zoom = 1,
        shiftX = 0,
        shiftY = 0;
    float zoomAngle = 0;
    float recentShiftX = 0,
        recentShiftY = 0;
    bool wasMouseMoved;
    bool vertexHighlight = false;
    int arrowShiftSpeed = 20;
    int pixelRadiusClickAreaSearch = 100;

    //SELECTIONS
    std::map<long, Vertex*> selectedVertices;
    std::list<Vertex*> selectedVerticesOrder;
    std::map<unsigned long, Edge*> selectedEdges;

    //SELECTIONS FOR OPENGL
    float* preparedEdges = nullptr;
    float* edgesColors = nullptr;
    float* vertexTriangles = nullptr;
    float* vtColors = nullptr;

    //METHODS
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


    void restoreDefaultView();

    void drawEdges(QOpenGLFunctions* f) const;
    void highlightVertices(QOpenGLFunctions* f) const;

    void prepareEdgesToDraw();
    void prepareVertexToDraw();

    void clearSelectedVertices();
    void clearSelectedEdges();

    bool selectVertex(Vertex* v);
    bool selectEdge(Edge *e);

    void linkAllSelectedVerticesTogether();
    void linkSelectedVerticesSequentially();

    void removeSelectedVertices();
    void removeSelectedEdges();

    QVector2D convertPointFromMapToCanvas(const QVector2D &v);
    QVector2D convertPointFromCanvasToMap(const QVector2D &v);
    QVector2D convertFromCoordsToMap(const QVector2D& v);
    QVector2D convertFromMapToCoords(const QVector2D& v);
    float convertDistFromPixToCoords(int d);

    QVector2D convertCurrentPointFromMapToCoords(const QVector2D& v);
    QVector2D convertCurrentPointFromCoordsToMap(const QVector2D& v);
};


#endif //PATHFINDING_PROBLEM_CUSTOMOPENGLWIDGET_H
