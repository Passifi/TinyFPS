#version 330 core
(location = 0) vec3 pos; 


void main() {
    gl_Position = vec4(pos,1.0);
}