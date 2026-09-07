#include "../include/material.h"

void Material::setValues() {
    if(texture) {
        texture->bind();
    } 
    if(shader) {
        shader->setFloatUniform("ambient",ambient);
        shader->setFloatUniform("diffuse",diffuse);
        shader->setFloatUniform("specular",specular);
        shader->setVec3Uniform("color",glm::vec3(color.red,color.green,color.blue));
    }
}