#pragma once

#include <GL/glew.h>

class ShaderLoader {
public:
    static GLuint load_shaders(const char * vertex_file_path,const char * fragment_file_path);
};

