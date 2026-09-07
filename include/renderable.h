#pragma once
#include "material.h"
#include "mesh.h"
#include "graphics.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Renderable {
  Shader& shader;
  Mesh& mesh;
  Texture* texture; 
  Material *material = nullptr;
  glm::vec3* transform = nullptr;
  glm::vec3* dimension = nullptr;
  Renderable(Shader& shader, Mesh& mesh, Material* material, Texture* texture,glm::vec3 * transform, glm::vec3* dimension);
  void draw();

};
