#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
uniform vec2 screenRatio;
uniform vec2 vSize;

void main() {
    vec2 size = vSize / screenRatio * 2;

    gl_Position = gl_in[0].gl_Position;
    gl_Position.y += size.y;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position;
    gl_Position.x -= size.x;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position;
    gl_Position.x += size.x;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position;
    gl_Position.y -= size.y;
    EmitVertex();
    EndPrimitive();
}
