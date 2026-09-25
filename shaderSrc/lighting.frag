#version 330 core 
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


out vec4 FragmentColor;

uniform Material material;
uniform vec3 objectColor;
uniform vec3 lightColor; 
uniform vec3 lightPos;
uniform vec3 viewPos;
in vec3 FragPos;
in vec3 Normal;
void main() {
    
    vec3 ambient = material.ambient*lightColor;
    float specularStrength = 0.9;
    // reflecton on normal 
    vec3 viewDir = normalize(viewPos-FragPos);
    float ambientStrength = 0.1;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 reflection = reflect(-lightDir,norm);

    float spec = pow(max(dot(viewDir,reflection),0.0),material.shininess); 
    vec3 specular = lightColor* (spec*material.specular); 
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = (diff*material.diffuse)*lightColor;
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragmentColor = vec4(result,1.0);
}