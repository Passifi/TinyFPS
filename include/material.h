#pragma once 
#include "texture.h"


struct Color {
  float red;
  float green;
  float blue;
  float alpha;
};
struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    Color color;
};

