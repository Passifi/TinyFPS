#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "camera.h"
#include "renderable.h"
#include "material.h"
#define color(c) c.red,c.green,c.blue,c.alpha



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

