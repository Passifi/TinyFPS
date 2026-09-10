#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector> 
#include <array>
#include <stdint.h>
#include <ctype.h>
#include <iostream>
#include <memory>
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

struct Vertex {
  glm::vec3 position; 
  glm::vec3 normals;
  glm::vec2 texCoord;
  Vertex(std::array<float,8> values) {
    position.x = values[0];
    position.y = values[1];
    position.z = values[2];
    normals.x = values[3];
    normals.y = values[4];
    normals.z = values[5];
    texCoord.x = values[6];
    texCoord.y = values[7];
  }
};


struct Mesh {
    std::vector<float> vertices;
  std::vector<unsigned int> indices;
  std::vector<unsigned int> vertexAttribInfo {3};
  unsigned int stride =6;
  unsigned int vao;
  unsigned int vbo;
  unsigned int ebo;
  Mesh(std::vector<Vertex> vertices); 
  Mesh(std::vector<float> vertices);
  Mesh(std::vector<float> vertices, std::vector<unsigned int> indices);
  void setVertexAttributes();  
  void intialize();  
  void bindVAO();
}; 


class MeshFactory {
    std::vector<std::unique_ptr<Mesh>> meshes;
  public:

    Mesh* addMesh(std::vector<Vertex> vertices) {
      auto mesh = std::make_unique<Mesh>(vertices);   
      auto ptr = mesh.get();
      meshes.push_back(std::move(mesh));
      return ptr;
    }

  ~MeshFactory() {

  }

};