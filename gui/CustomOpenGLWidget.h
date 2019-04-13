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
#include <QColorDialog>
#include <gui/VertexButton.h>
#include <classes/Algorithms.h>
#include "../classes/Graph/Graph.h"
#include "classes/Category.h"
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

    // Processing results
    void processResult(const AlgResult*);
    void dropCurrentResultSl();

    // Result view
    void hlPathChanged(bool);
    void hlFSPChanged(bool);
    void hlRSPChanged(bool);
    void hlFSAChanged(bool);
    void hlRSAChanged(bool);

    void pathColorChanged(QColor);
    void fspColorChanged(QColor);
    void rspColorChanged(QColor);
    void fsaColorChanged(QColor);
    void rsaColorChanged(QColor);

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

    // Changing colors
    void changeVertexColor(const QColor& color);
    void changeSelectedVertexColor(const QColor& color);
    void changeEdgeColor(const QColor& color);
    void changeSelectedEdgeColor(const QColor& color);


private slots:
    void checkForEnablingSearchButton(unsigned long, unsigned long, unsigned long,unsigned long);

signals:
    void save();
    void amountsChanged(unsigned long, unsigned long, unsigned long,unsigned long);
    void clickModeChangedByKey(uint);
    void highlightSig(bool);
    void enableToSearchPath(bool);

private:
    //INTERFACE
    MapInfo* info;
    MultiToggleButton* mtb;
    QSlider* vertexSizeSlider;
    uint clickMode;

    //OPENGL INFO
    Graph* graph;
    int initialWidth, initialHeight;
    float vertexRadius;
    float zoom,
        shiftX,
        shiftY;
    float zoomAngle;
    float recentShiftX,
        recentShiftY;
    bool wasMouseMoved;
    bool vertexHighlight;
    int arrowShiftSpeed;
    int pixelRadiusClickAreaSearch;

    // TO DRAW
    QOpenGLShaderProgram* edge_default_shader_program;
    void drawEdges(QOpenGLShaderProgram*, const QColor&, const float*, int) const;
    void drawAllEdges();

    CategoriesContainer<Edge> edgeHandler;
    void initEdgeHandler();

    QOpenGLShaderProgram* vertex_default_shader_program;
    void drawVertices(QOpenGLShaderProgram*, const QColor&, const float*, int) const;
    void drawAllVertices();

    CategoriesContainer<Vertex> vertexHandler;
    void initVertexHandler();

    // FOR ALGORITHM RESULTS
    const AlgResult* algResult = nullptr;
    bool hlPath = true,
         hlForwardPath = true,
         hlReversePath = true,
         hlForwardEdges = true,
         hlReverseEdges = true;

    void initAlgResult(const AlgResult*);
    void drawAlgResult();
    void dropAlgResult();
    // END OF TO DRAW

    // Colors
    QColor vertexColor;
    QColor selectedVertexColor;

    QColor edgeColor;
    QColor selectedEdgesColor;

    QColor pathColor;
    QColor forwardSearchPathEdgesColor;
    QColor reverseSearchPathEdgesColor;
    QColor forwardSearchEdgesColor;
    QColor reverseSearchEdgesColor;

    //SELECTIONS
    /*std::map<long, Vertex*> selectedVertices;
    std::list<Vertex*> selectedVerticesOrder;*/
    std::map<unsigned long, Edge*> selectedEdges;


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


    template <class T> friend class CategoriesContainer;
    friend class MainWindow;
};


#endif //PATHFINDING_PROBLEM_CUSTOMOPENGLWIDGET_H
