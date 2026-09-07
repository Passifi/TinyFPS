#pragma once 
#include "shader.h"
#include "graphics.h"
struct Material {
    Shader* shader = nullptr;
    Color color;
    float specular;
    float diffuse;
    float ambient;
    Texture* texture = nullptr; 
    void setValues();;
};