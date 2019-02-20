#version 330 core

layout(location = 0) highp in vec2 pos;
uniform vec2 minxy;
uniform vec2 maxxy;
uniform float zoom;
uniform vec2 shiftInPix;
uniform vec2 screenRatio;


void main(){
    vec2 shift = shiftInPix / screenRatio * (maxxy - minxy);
    shift.y = - shift.y;
    gl_Position.xy = ((pos + shift - minxy) / (maxxy - minxy) * 2 - 1) * zoom;
    gl_Position.z = 0;
    gl_Position.w = 1.0;
}