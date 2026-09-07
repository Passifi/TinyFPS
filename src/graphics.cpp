#include "../include/graphics.h"

void Renderer::setFOV(float fov) {
      this->fov = fov;
   }
void Renderer::modifyFOV(float offset) {
    this->fov += offset;
    if(this->fov < TinyRenderer::MIN_FOV) {
      this->fov = TinyRenderer::MIN_FOV;
    } else if
    (this->fov > TinyRenderer::MAX_FOV ) {
      this->fov = TinyRenderer::MAX_FOV;
      }
   }
   void Renderer::toggleProjectionState() {
    perspectiveState = Perspective ? Orthographic : Perspective;
   }
void Renderer::setViewport() {
    setViewport(fov,(float)screenWidth,(float)screenHeight);
   }
   void Renderer::setScreenDimensions(uint width, uint height) {
    this->screenWidth = width;
    this->screenHeight = height;
   }
   void Renderer::setViewport(float fov,float width,float height) {

      if(perspectiveState == Orthographic)
        projection = glm::ortho(0.0f,width,0.0f,height); 
      else 
        projection = glm::perspective(glm::radians(fov),width/height,0.1f,100.0f);
   }
   void Renderer::initialize() {
      glClearColor(color(backgroundColor));
      glEnable(GL_DEPTH_TEST);
      setViewport(45.0f,800.0f,600.0f);
   }
   void Renderer::addRenderable(Renderable*  renderable) {
      renderables.push_back(renderable); 
    }
    void Renderer::render() {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glm::mat4 view = camera.getView();
      glm::mat4 baseMat(1.0f);
      for(auto&el : renderables) {
        glm::mat4 translation = glm::translate(baseMat,*el->transform);
        if(el->dimension)
         translation = glm::scale(translation,*el->dimension);
        glm::mat4 mvp = translation;
        if(el->material) {
            el->shader.setFloatUniform("diffuse",el->material->diffuse);
            el->shader.setFloatUniform("ambient",el->material->ambient);
            el->shader.setFloatUniform("specular",el->material->specular);
        }
        el->shader.setMat4Uniform("mvp",projection*view*mvp); 
        el->shader.setVec3Uniform("objectColor",{0.3f,0.2f,0.2f});
        el->shader.setVec3Uniform("lightColor",{0.9f,0.2f,0.1});
        el->draw();
      }
    }