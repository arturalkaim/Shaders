#version 410 core
in vec2 pos;
in float type;
in float sides;

uniform float time;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out int vType;
out float vTime;
out vec2 vPos;
out int vSides;

void main() {
    gl_Position = vec4(pos, 0.0, 1.0); //model * view * projection * 
    vType = int(type);
    vSides = int(sides);
    vTime = time;
    vPos = pos;
}