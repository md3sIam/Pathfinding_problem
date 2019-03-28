//
// Created by md3s on 27.03.19.
//

#ifndef PATHFINDING_PROBLEM_COLORSELECTBUTTON_H
#define PATHFINDING_PROBLEM_COLORSELECTBUTTON_H
#include <QPushButton>
#include <QColorDialog>

class ColorSelectButton : public QPushButton{
    Q_OBJECT
public:
    explicit ColorSelectButton(QWidget* parent = nullptr) : QPushButton(parent){
        connect(this, SIGNAL(clicked()), this, SLOT(getColor()));
    }
    explicit ColorSelectButton(const QString& str, QWidget* parent = nullptr) : ColorSelectButton(parent){
        setText(str);
    }

signals:
    void colorSelected(const QColor&);

private slots:
    void getColor(){emit colorSelected(QColorDialog::getColor());};

};

#endif //PATHFINDING_PROBLEM_COLORSELECTBUTTON_H