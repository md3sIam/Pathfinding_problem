#version 330 core

layout(location = 0) highp in vec2 pos;
attribute vec2 minxy;
attribute vec2 maxxy;


void main(){
    gl_Position.xy = (pos - minxy) / (maxxy - minxy) * 2 - 1;
    gl_Position.z = 0;
    gl_Position.w = 1.0;
}