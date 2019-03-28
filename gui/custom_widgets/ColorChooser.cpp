//
// Created by md3s on 28.03.19.
//

#include <QtWidgets/QHBoxLayout>
#include <iostream>
#include "ColorChooser.h"

ColorChooser::ColorChooser(QWidget *parent) : QFrame(parent) {
    setContentsMargins(0,0,0,0);

    layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);

    button = new ColorSelectButton(this);
    demo_frame = new QFrame(this);
    demo_frame->setFrameShape(QFrame::Box);

    button->setFocusPolicy(Qt::NoFocus);

    layout->addWidget(button);
    layout->addWidget(demo_frame);
    demo_frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    current_color = QColor(0,0,0);

    setLayout(layout);

    connect(button, SIGNAL(colorSelected(const QColor&)), this, SLOT(setDemoColor(const QColor&)));
}

ColorChooser::ColorChooser(const QString &str, const QColor &color, QWidget *parent) : ColorChooser(parent){
    button->setText(str);
    setDemoColor(color);
}

void ColorChooser::setColor(const QColor &color) {
    QString str = "Background: rgb(";
    str += QString(std::to_string(color.red()).c_str()) + QString(",")
            + QString(std::to_string(color.green()).c_str()) + QString(",")
            + QString(std::to_string(color.blue()).c_str()) + QString(");") ;
    std::cout << str.toStdString();
    current_color = color;
    demo_frame->setStyleSheet(str);
    emit colorChanged(current_color);
}

void ColorChooser::setStr(const QString &str) {
    button->setText(str);
}

void ColorChooser::setDemoColor(const QColor &color) {
    setColor(color);
}

