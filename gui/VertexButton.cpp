#include "VertexButton.h"
#include <iostream>
#include <QPainter>
#include <QVector2D>

VertexButton::VertexButton(int _x, int _y, QWidget* par) : QAbstractButton(par) {
    x = _x;
    y = _y;
    move(x, y);
    setMinimumHeight(10);
    setMinimumWidth(10);
    setMaximumHeight(10);
    setMaximumWidth(10);
}

VertexButton::VertexButton(const QVector2D &vec, QWidget *parent) :
    QAbstractButton(parent), x(vec.x()), y(vec.y()){

    move(x, y);
    setMinimumHeight(10);
    setMinimumWidth(10);
    setMaximumHeight(10);
    setMaximumWidth(10);
}

void VertexButton::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    painter.setPen(Qt::green);
    painter.setBrush(Qt::SolidPattern);
    painter.setBrush(Qt::green);
    painter.setBackground(Qt::SolidPattern);
    painter.drawRoundedRect(0, 0, 10, 10, 10, 10);
}

void VertexButton::mousePressEvent(QMouseEvent *e) {
    std::cout << "clicked\n";
}