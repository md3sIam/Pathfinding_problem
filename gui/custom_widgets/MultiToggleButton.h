//
// Created by md3s on 01.03.19.
//

#ifndef PATHFINDING_PROBLEM_TRIBUTTON_H
#define PATHFINDING_PROBLEM_TRIBUTTON_H

#include <QHBoxLayout>
#include <QPushButton>
#include <QVector>

class MultiToggleButton : public QWidget {
    Q_OBJECT
private:
    uint state;
    QVector<QPushButton*> buttons;
    QHBoxLayout* layout;

public:
    MultiToggleButton(QVector<QString> names, QWidget* parent = 0);
    ~MultiToggleButton();
    inline uint getState(){ return state; }


public slots:
    void changeValue();
    void changeValue(uint);

signals:
    void valueChanged(uint state);

};


#endif //PATHFINDING_PROBLEM_TRIBUTTON_H
