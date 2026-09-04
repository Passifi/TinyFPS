#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector> 
#include <stdint.h>
#include <ctype.h>
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

struct Vertex {
  glm::vec3 position; 
  glm::vec3 normals;
  glm::vec2 texCoord;
};


struct Mesh {
    std::vector<float> vertices;
  std::vector<unsigned int> indices;
  std::vector<unsigned int> vertexAttribInfo {3,2};
  unsigned int stride =5;
  unsigned int vao;
  unsigned int vbo;
  unsigned int ebo;
 
  Mesh(std::vector<float> vertices) : vertices(vertices)  {
    
  }
  Mesh(std::vector<float> vertices, std::vector<unsigned int> indices) : vertices(vertices), indices(indices) {}
 void setVertexAttributes() {
    if(vertexAttribInfo.size() == 0) {
      glVertexAttribPointer(0,stride,GL_FLOAT,GL_FALSE,stride*sizeof(float),(void*)0);
      glEnableVertexAttribArray(0);
    }
    else {
        std::cout << "Initi vertex attrib pointers" << std::endl;
      int attributeArrayIndex = 0;
      int currentLength = 0;
      for(auto& length : vertexAttribInfo) {
        glVertexAttribPointer(attributeArrayIndex,length,GL_FLOAT,GL_FALSE, stride*sizeof(float),(void*)(currentLength*sizeof(float)));
        glEnableVertexAttribArray(attributeArrayIndex++);
        currentLength += length;
      }
   }
  } 
  void intialize() {
      glGenBuffers(1,&vbo);
      glGenBuffers(1,&ebo);
      glGenVertexArrays(1,&vao);
      glBindVertexArray(vao);
      glBindBuffer(GL_ARRAY_BUFFER,vbo);
      glBufferData(GL_ARRAY_BUFFER,sizeof(float)*vertices.size(),vertices.data(),GL_STATIC_DRAW);
  
      if(indices.size() != 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*indices.size(),indices.data(),GL_STATIC_DRAW);
      }
      setVertexAttributes();
  }

  void bindVAO() {
    glBindVertexArray(vao);
  }
}; 