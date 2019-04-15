//
// Created by md3s on 31.03.19.
//

#include <iostream>
#include <classes/Edge/Edge.h>
#include "Category.h"
#include "../gui/CustomOpenGLWidget.h"

template <class T>
Category<T>::Category(const QColor &c, const std::string &str) {
    color = c;
    name = str;
    is_changed = false;
}

template <class T>
void Category<T>::setColor(const QColor &c) {
    color = c;
}

template <class T>
void Category<T>::setName(const std::string &str) {
    name = str;
}

template <class T>
void Category<T>::add(T* v) {
    dict.insert({v->id, v});
    sequence.push_back(v);
    is_changed = true;
}

template <class T>
void Category<T>::clear() {
    dict.clear();
    sequence.clear();
    is_changed = true;
}

template <class T>
void CategoriesContainer<T>::clearType(const std::string &str) {
    types[str]->clear();
}

template <>
void Category<Vertex>::prepareToDraw(float **array) const {
    delete [] *array;
    if (sequence.size() == 0){
        *array = nullptr;
        return;
    }

    (*array) = new float[2 * sequence.size()];
    unsigned i = 0;
    for (const Vertex *vertex : sequence) {
        (*array)[i] = vertex->lon;
        (*array)[i + 1] = vertex->lat;
        i += 2;
    }
}

template <>
void Category<Edge>::prepareToDraw(float **array) const {
    delete [] *array;
    if (sequence.size() == 0){
        *array = nullptr;
        return;
    }
    (*array) = new float[4 * sequence.size()];
    unsigned i = 0;
    for (const Edge* edge : sequence) {
        (*array)[i] = edge->vFrom->lon;
        (*array)[i + 1] = edge->vFrom->lat;
        (*array)[i + 2] = edge->vTo->lon;
        (*array)[i + 3] = edge->vTo->lat;
        i += 4;
        }

}

template <class T>
CategoriesContainer<T>::~CategoriesContainer() {
    for (auto item : arrayToDraw)
        delete [] item.second;
    for (auto item : types)
        delete item.second;
}

template <class T>
void CategoriesContainer<T>::createType(const std::string &str, const QColor &color) {
    auto type = new Category<T>(color, str);
    types.insert({str, type});
    arrayToDraw.insert({str, nullptr});
}

template <class T>
void CategoriesContainer<T>::removeType(const std::string &str) {
    types.erase(types.find(str));
    delete [] arrayToDraw[str];
    arrayToDraw.erase(arrayToDraw.find(str));
}

template <class T>
Category<T>& CategoriesContainer<T>::operator[](const std::string &str) {
    return *types[str];
}

template <class T>
void CategoriesContainer<T>::updateArray(const std::string &str) {
    delete[] arrayToDraw[str];
    types[str]->prepareToDraw(&arrayToDraw[str]);
}

template <>
void CategoriesContainer<Vertex>::drawType(const std::string &str, const CustomOpenGLWidget *widget, bool wider) {
    if (types[str]->is_changed) {
        types[str]->prepareToDraw(&arrayToDraw[str]);
        types[str]->is_changed = false;
    }

    if (types[str]->getSize() == 0) return;
    widget->drawVertices(widget->vertex_default_shader_program,
                         types[str]->color, arrayToDraw[str], types[str]->getSize());
}

template <>
void CategoriesContainer<Edge>::drawType(const std::string &str, const CustomOpenGLWidget *widget, bool wider) {
    if (types[str]->is_changed) {
        types[str]->prepareToDraw(&arrayToDraw[str]);
        types[str]->is_changed = false;
    }

    if (types[str]->getSize() == 0) return;
    if (!wider)
        widget->drawEdges(widget->edge_default_shader_program,
                         types[str]->color, arrayToDraw[str], 2 * types[str]->getSize());
    else
        widget->drawEdges(widget->wider_edge_shader_program,
                          types[str]->color, arrayToDraw[str], 2 * types[str]->getSize());
}

template <class T>
void CategoriesContainer<T>::draw(const CustomOpenGLWidget *widget) {
    for (const auto& pair: types){
        drawType(pair.first, widget);
    }
}

template <class T>
void CategoriesContainer<T>::clear() {
    for (const auto& pair : types)
        removeType(pair.first);
}

template <class T>
void CategoriesContainer<T>::addToType(const std::string &str, T *v) {
    types[str]->add(v);
}

template <class T>
void CategoriesContainer<T>::removeFromType(const std::string &str, T *v) {
    types[str]->remove(v);
}

template <class T>
void Category<T>::remove(const T* v) {
    auto it = dict.find(v->id);
    if (it != dict.end()) {
        dict.erase(it);
        for (auto it = sequence.begin(); it != sequence.end(); it++)
            if (*it == v) {
                sequence.erase(it);
                break;
            }
        is_changed = true;
    }
}

template <class T>
bool CategoriesContainer<T>::typeContains(const std::string &str, T *v) {
    return types[str]->typeContains(v);
}

template <class T>
bool Category<T>::typeContains(const T* v) {
    if (dict.find(v->id) == dict.end())
        return false;
    else return true;
}

template <class T>
void CategoriesContainer<T>::setColorToType(const std::string &str, const QColor &color) {
    types[str]->setColor(color);
}

template <class T>
void CategoriesContainer<T>::add(T *el) {
    for (auto it = types.begin(); it != types.end(); it++){
        addToType((*it).first, el);
    }
}

template <class T>
void CategoriesContainer<T>::remove(T *el) {
    for (auto it = types.begin(); it != types.end(); it++){
        removeFromType((*it).first, el);
    }
}

template class CategoriesContainer<Vertex>;
template class CategoriesContainer<Edge>;