#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec3 color;
uniform vec2 minxy;
uniform vec2 maxxy;
uniform float zoom;
uniform vec2 shiftInPix;
uniform vec2 screenRatio;
out vec3 g_color;

void main(){
    vec2 delta = maxxy - minxy;
    vec2 shift = shiftInPix / screenRatio * delta;
    shift.y = - shift.y;
    gl_Position.xy = ((pos + shift - minxy) / delta * 2 - 1) * zoom;
    g_color = color;
}
