#version 330 core

layout(location = 0) highp in vec2 pos;
uniform vec2 minxy;
uniform vec2 maxxy;
attribute float zoom;


void main(){
    gl_Position.xy = ((pos - minxy) / (maxxy - minxy) * 2 - 1) * zoom;
    gl_Position.z = 0;
    gl_Position.w = 1.0;
}