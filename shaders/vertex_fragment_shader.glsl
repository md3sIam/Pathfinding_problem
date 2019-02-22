#version 330 core

uniform bool f;
out vec3 color;

void main() {
    if (!f) {
        color.xyz = vec3(0.0, 0.0, 1.0f);
    }
    else {
        color.xyz = vec3(0.0, 1.0, 0.0);
    }
}
