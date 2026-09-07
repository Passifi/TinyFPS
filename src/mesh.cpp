#include "../include/mesh.h"
  Mesh::Mesh(std::vector<Vertex> vertices) {
    for(auto& vertex : vertices) {
        this->vertices.push_back(vertex.normals.x);
        this->vertices.push_back(vertex.normals.y);
        this->vertices.push_back(vertex.normals.z);
        this->vertices.push_back(vertex.position.x);
        this->vertices.push_back(vertex.position.y);
        this->vertices.push_back(vertex.position.z);
        this->vertices.push_back(vertex.texCoord.x);
        this->vertices.push_back(vertex.texCoord.y);
    }
  }
  Mesh::Mesh(std::vector<float> vertices) : vertices(vertices)  {
    
  }
  Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned int> indices) : vertices(vertices), indices(indices) {}
 
  void Mesh::setVertexAttributes() {
    if(vertexAttribInfo.size() == 0) {
      glVertexAttribPointer(0,stride,GL_FLOAT,GL_FALSE,stride*sizeof(float),(void*)0);
      glEnableVertexAttribArray(0);
    }
    else {
      int attributeArrayIndex = 0;
      int currentLength = 0;
      for(auto& length : vertexAttribInfo) {
        glVertexAttribPointer(attributeArrayIndex,length,GL_FLOAT,GL_FALSE, stride*sizeof(float),(void*)(currentLength*sizeof(float)));
        glEnableVertexAttribArray(attributeArrayIndex++);
        currentLength += length;
      }
   }
  } 
  void Mesh::intialize() {
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

  void Mesh::bindVAO() {
    glBindVertexArray(vao);
  }
