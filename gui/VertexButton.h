#ifndef PATHFINDING_PROBLEM_VERTEXBUTTON_H
#define PATHFINDING_PROBLEM_VERTEXBUTTON_H

#include <../classes/Vertex/Vertex.h>
#include <QAbstractButton>


class VertexButton : public QAbstractButton{

private:
    Vertex* vertex;
    int x, y;

public:
    VertexButton(Vertex* v);
    VertexButton(int x, int y, QWidget* parent = 0);
    VertexButton(const QVector2D& vec, QWidget* parent = 0);

protected:
    void paintEvent(QPaintEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
};


#endif //PATHFINDING_PROBLEM_VERTEXBUTTON_H
