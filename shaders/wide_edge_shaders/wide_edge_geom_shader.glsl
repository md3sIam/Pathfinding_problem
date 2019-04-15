#version 330 core

layout(lines) in;
layout(triangle_strip, max_vertices = 10) out;
uniform vec2 baseRatio;

void main() {
    vec2 vector = gl_in[1].gl_Position.xy - gl_in[0].gl_Position.xy;
    float len_v = length(vector);
    float cos_v = vector.x / len_v;
    float sin_v = vector.y / len_v;
    float add_x_mod = 3f * sin_v / baseRatio.x;
    float add_y_mod = 3f * cos_v / baseRatio.y;

//    points[0] = gl_in[0].gl_Position + vec2(-add_x_mod, add_y_mod);
//    points[1] = gl_in[0].gl_Position + vec2(add_x_mod, -add_y_mod);
//    points[2] = gl_in[1].gl_Position + vec2(-add_x_mod, add_y_mod);
//    points[3] = gl_in[1].gl_Position + vec2(add_x_mod, -add_y_mod);

    gl_Position.xy = gl_in[0].gl_Position.xy + vec2(add_x_mod, -add_y_mod);
    EmitVertex();
    gl_Position.xy = gl_in[0].gl_Position.xy + vec2(-add_x_mod, add_y_mod);
    EmitVertex();
    gl_Position.xy = gl_in[1].gl_Position.xy + vec2(add_x_mod, -add_y_mod);
    EmitVertex();
//    EndPrimitive();
    gl_Position.xy = gl_in[1].gl_Position.xy + vec2(-add_x_mod, add_y_mod);
    EmitVertex();
    EndPrimitive();
}
