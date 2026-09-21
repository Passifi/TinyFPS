#pragma once
#include "mesh.h"
#include "material.h"
#include "shader.h"
#include "texture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>



struct Renderable {
  Shader& shader;
  Mesh& mesh;
  Texture* texture; 
  Material *material = nullptr;
  glm::vec3* transform = nullptr;
  glm::vec3* dimension = nullptr;
  glm::vec3* lightSource = nullptr; 
  Renderable(Shader& shader, Mesh& mesh, Material* material, Texture* texture,glm::vec3 * transform, glm::vec3* dimension);
  void draw();

};
