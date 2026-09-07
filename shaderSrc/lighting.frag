#version 330 core 
out vec4 FragColor; 

uniform vec3 objectColor;
uniform vec3 lightColor; 
uniform float diffuse;
uniform float specular;
uniform float ambient;
void main() 
{
    FragColor = vec4(objectColor*lightColor*diffuse,1.0);
}