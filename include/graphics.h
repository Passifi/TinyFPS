#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "../include/camera.h"
#include "../include/renderable.h"
#include "material.h"
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

enum RendererPerspective {
 Orthographic, 
 Perspective
};

namespace TinyRenderer{ 
  const float MAX_FOV = 60.0f;
  const float MIN_FOV = 1.0f;
}

class Renderer {

  Color backgroundColor{0.9,0.1,0.8,1.0};
  std::vector<Renderable*> renderables;
  glm::mat4 projection;
  uint screenWidth = 800;
  uint screenHeight = 600;
  float fov = 45.0f;
  RendererPerspective perspectiveState = Perspective;
  public:

    Camera camera;
    Renderer() {
    }
   void setFOV(float fov) {
      this->fov = fov;
   }
   void modifyFOV(float offset) {
    this->fov += offset;
    if(this->fov < TinyRenderer::MIN_FOV) {
      this->fov = TinyRenderer::MIN_FOV;
    } else if
    (this->fov > TinyRenderer::MAX_FOV ) {
      this->fov = TinyRenderer::MAX_FOV;
      }
   }
   void toggleProjectionState() {
    perspectiveState = Perspective ? Orthographic : Perspective;
   }
   void setViewport() {
    setViewport(fov,(float)screenWidth,(float)screenHeight);
   }
   void setScreenDimensions(uint width, uint height) {
    this->screenWidth = width;
    this->screenHeight = height;
   }
   void setViewport(float fov,float width,float height) {

      if(perspectiveState == Orthographic)
        projection = glm::ortho(0.0f,width,0.0f,height); 
      else 
        projection = glm::perspective(glm::radians(fov),width/height,0.1f,100.0f);
   }
   void initialize() {
      glClearColor(color(backgroundColor));
      glEnable(GL_DEPTH_TEST);
      setViewport(45.0f,800.0f,600.0f);
   }
   void addRenderable(Renderable*  renderable) {
      renderables.push_back(renderable); 
    }

    
    void render() {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glm::mat4 view = camera.getView();
      glm::mat4 baseMat(1.0f);
      for(auto&el : renderables) {
        glm::mat4 translation = glm::translate(baseMat,*el->transform);
        if(el->dimension)
         translation = glm::scale(translation,*el->dimension);
        glm::mat4 mvp = translation;
        el->material->setValues();
        el->shader.setMat4Uniform("mvp",projection*view*mvp); 
        el->shader.setVec3Uniform("objectColor",{0.3f,0.2f,0.2f});
        el->shader.setVec3Uniform("lightColor",{0.9f,0.2f,0.1});
        el->draw();
      }
    }
};

