#version 330 core

in vec3 f_color;
uniform bool f;
out vec3 color;

void main() {
    color = f_color;
}
