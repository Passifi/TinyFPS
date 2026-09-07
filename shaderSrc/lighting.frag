#version 330 core 
out vec4 FragColor;
in vec3 Normal;
struct Material {
 vec3 ambient; 
 vec3 diffuse;
 vec3 specular;
 float shininess;
};


uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 color;
void main() 
{
    FragColor = vec4(objectColor*lightColor*diffuse,1.0);
}