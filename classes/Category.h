//
// Created by md3s on 31.03.19.
//

#ifndef PATHFINDING_PROBLEM_VERTEXTYPE_H
#define PATHFINDING_PROBLEM_VERTEXTYPE_H


#include <classes/Vertex/Vertex.h>
#include <QtGui/QColor>
#include <unordered_map>

class CustomOpenGLWidget;

template <class T>
class Category {
    std::string name;
    QColor color;
    std::unordered_map<unsigned long, const T*> dict;
    std::vector<T*> sequence;
    bool is_changed;

public:
    Category(const QColor& color, const std::string& name = "");
    ~Category() = default;

    void setColor(const QColor& color);
    void setName(const std::string& str);
    void add(T* v);
    void remove(const T* v);
    bool typeContains(const T* v);
    void clear();

    const std::vector<T*>& getSequence(){return sequence;}
    unsigned getSize() {return sequence.size();}
    std::string& getName() {return name;}
    QColor& getColor(){return color;}
    void prepareToDraw(float** array) const;

    template <class T1> friend class CategoriesContainer;
};



template <class T>
class CategoriesContainer {
    std::unordered_map<std::string, Category<T>*> types;
    std::unordered_map<std::string, float*> arrayToDraw;

    void updateArray(const std::string& str);

public:
    CategoriesContainer() = default;
    ~CategoriesContainer();

    void createType(const std::string& str, const QColor& color);
    Category<T>& operator[](const std::string& str);
    void removeType(const std::string& str);
    void clearType(const std::string& str);

    void addToType(const std::string&, T* v);
    void removeFromType(const std::string&, T* v);
    bool typeContains(const std::string& str, T* v);
    void setColorToType(const std::string& str, const QColor& color);

    void add(T*);
    void remove(T*);

    void clear();
    void drawType(const std::string& str, const CustomOpenGLWidget* widget);
    void draw(const CustomOpenGLWidget* widget);
};



#endif //PATHFINDING_PROBLEM_VERTEXTYPE_H
