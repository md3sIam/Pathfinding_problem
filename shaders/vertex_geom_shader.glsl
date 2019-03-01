#version 330 core
layout(points) in;
in vec3 g_color[];
layout(triangle_strip, max_vertices = 48) out;
out vec3 f_color;
uniform vec2 screenRatio;
uniform float radius;

void drawCircles();
void drawSquares();

void main() {
    drawCircles();
}

void drawCircles(){
    vec2 size = vec2(radius, radius) / screenRatio * 2;
    vec4 points[16];
    vec4 center = gl_in[0].gl_Position;
    float deltaPi = 3.1415f / 8;
    for (int i = 0; i < 16; i++){
        points[i].x = center.x + size.x * cos(deltaPi * i);
        points[i].y = center.y + size.y * sin(deltaPi * i);
        points[i].zw = vec2(0, 1);
    }
    for (int i = 0; i < 16; i++){
        gl_Position = center;
        f_color = g_color[0];
//        f_color = vec3(1);
        EmitVertex();

        gl_Position = points[i];
        f_color = g_color[0];
        f_color *= 0.98;
        EmitVertex();

        gl_Position = points[(i + 1) % 16];
        f_color = g_color[0];
        f_color *= 0.98;
        EmitVertex();
        EndPrimitive();
    }
}

void drawSquares(){
    vec2 size = vec2(radius, radius) / screenRatio * 2;
    gl_Position = gl_in[0].gl_Position;
    gl_Position.y += size.y;
    f_color = g_color[0].xyz;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position;
    gl_Position.x -= size.x;
    f_color = g_color[0].xyz;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position;
    gl_Position.x += size.x;
    f_color = g_color[0].xyz;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position;
    gl_Position.y -= size.y;
    f_color = g_color[0].xyz;
    EmitVertex();
    EndPrimitive();
}
