#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform vec4 timeColor;
out vec4 VertexColor;
void main() {
    VertexColor = vec4(aColor,1.0f);
    gl_Position = vec4(aPos.x, aPos.y, aPos.z,1.0f); 
}