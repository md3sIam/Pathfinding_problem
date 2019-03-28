//
// Created by md3s on 28.03.19.
//

#ifndef PATHFINDING_PROBLEM_COLORCHOOSER_H
#define PATHFINDING_PROBLEM_COLORCHOOSER_H
#include <QFrame>
#include "ColorSelectButton.h"

class ColorChooser : public QFrame {

    Q_OBJECT

private:
    QLayout* layout;
    ColorSelectButton* button;
    QFrame* demo_frame;
    QColor current_color;

private slots:
    void setDemoColor(const QColor& color);

public:
    ColorChooser(QWidget* parent = nullptr);
    ColorChooser(const QString& str, const QColor& color, QWidget* parent = nullptr);

    void setStr(const QString& str);
    void setColor(const QColor& color);

signals:
    void colorChanged(QColor);
};


#endif //PATHFINDING_PROBLEM_COLORCHOOSER_H
