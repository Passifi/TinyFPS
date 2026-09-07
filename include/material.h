#pragma once 
#include "texture.h"


struct Color {
  float red;
  float green;
  float blue;
  float alpha;
};
struct Material {
    float specular;
    float diffuse;
    float ambient;
    Color color;
    Texture* texture = nullptr; 
};