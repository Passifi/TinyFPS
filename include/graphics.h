#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "../include/camera.h"
#define color(c) c.red,c.green,c.blue,c.alpha

struct Color {
  float red;
  float green;
  float blue;
  float alpha;
};

struct TextureImage {
  int width,height,nrChannels;
  unsigned char* data;
};
TextureImage loadImageData(const char* source) {
  TextureImage img;
  img.data = stbi_load(source,&img.width,&img.height,&img.nrChannels,0);
  return img;
}


struct Texture {
  unsigned int id;
  Texture(TextureImage img) {
    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_2D,id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img.width,img.height,0,GL_RGB,GL_UNSIGNED_BYTE,img.data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  void bind() {
    glBindTexture(GL_TEXTURE_2D,id);
  }
   
};
struct Renderable {
  Shader& shader;
  Mesh& mesh;
  Texture* texture;
  glm::vec3* transform = nullptr;
  glm::vec3* dimension = nullptr;
  void draw() {
    shader.use();
    texture->bind(); 
    mesh.bindVAO();
    if(mesh.indices.size() > 0) 
    glDrawElements(GL_TRIANGLES,mesh.indices.size(),GL_UNSIGNED_INT,0);
    else 
      glDrawArrays(GL_TRIANGLES,0,36);
    glBindVertexArray(0);
  }
};

class Renderer {

  Color backgroundColor{0.9,0.1,0.8,1.0};
  std::vector<Renderable*> renderables;
  Camera camera;
  glm::mat4 projection;
  uint screenWidth;
  uint screentHeight;
  public:
    Renderer() {
    }
   void initialize() {
      glClearColor(color(backgroundColor));
      glEnable(GL_DEPTH_TEST);
   }
   void addRenderable(Renderable*  renderable) {
      renderables.push_back(renderable); 
    }
    void render() {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glm::mat4 projection = glm::mat4(1.0f);
      glm::mat4 view = camera.getView();
      projection = glm::perspective(glm::radians(45.f),800.0f/600.0f,0.1f,100.0f);
      glm::mat4 baseMat(1.0f);
      for(auto&el : renderables) {
        glm::mat4 translation = glm::translate(baseMat,*el->transform);
        if(el->dimension)
         translation = glm::scale(translation,*el->dimension);
        glm::mat4 mvp = translation;
        el->shader.setMat4Uniform("mvp",projection*view*mvp); 
        el->draw();
      }
    }
};

