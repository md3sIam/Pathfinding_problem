//
// Created by md3s on 06.07.19.
//

#ifndef PATHFINDING_PROBLEM_EXCEPTIONS_H
#define PATHFINDING_PROBLEM_EXCEPTIONS_H

#include <stdexcept>

class FileNotOpenedException : public std::exception {
protected:
    const char* msg = nullptr;
public:
    FileNotOpenedException() : std::exception(){}
    explicit FileNotOpenedException(const char* msg) : FileNotOpenedException(){
        this->msg = msg;
    }
    const char* what() const noexcept override {
        if (msg == nullptr) return "File is not found";
        else return msg;
    }
};


class ShaderNotLoadedException : public FileNotOpenedException {
public:
    ShaderNotLoadedException() : FileNotOpenedException(){}
    explicit ShaderNotLoadedException(const char* msg) : FileNotOpenedException(msg){}
    const char* what() const noexcept override {
        if (msg == nullptr) return "Shader is not loaded";
        else return msg;
    }
};

#endif //PATHFINDING_PROBLEM_EXCEPTIONS_H
