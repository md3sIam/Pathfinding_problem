//
// Created by md3s on 01.03.19.
//

#include "MultiToggleButton.h"
#include <iostream>

MultiToggleButton::MultiToggleButton(QVector<QString> names, QWidget *parent) : QWidget(parent) {
    layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    for (auto const &name : names){
        auto button = new QPushButton(name, parent);
        button->setObjectName(name);
        button->setCheckable(true);
        button->setChecked(false);
        connect(button, SIGNAL(clicked()),
                this, SLOT(changeValue()));
        buttons.push_back(button);
        layout->addWidget(button);
    }
    buttons[0]->setChecked(true);
    state = 0;
}

MultiToggleButton::~MultiToggleButton() {
    for (auto &button : buttons){
        delete button;
    }
}

void MultiToggleButton::changeValue() {
    if (!buttons[state]->isChecked()){
        buttons[state]->setChecked(true);
        std::cout << "Tried to change!\n";
        return;
    }

    buttons[state]->setChecked(false);
    for (uint i = 0; i < buttons.size(); i++){
        if (buttons[i]->isChecked()){
            state = i;
            emit valueChanged(state);
            std::cout << "Current state: " << state << std::endl;
            break;
        }
    }
}

