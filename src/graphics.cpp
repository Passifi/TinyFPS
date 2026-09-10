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
   void Renderer::setScreenDimensions(unsigned int width, unsigned int height) {
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
        el->shader.use();
        el->shader.setMat4Uniform("model",mvp);
        el->shader.setMat4Uniform("projection",projection);
        el->shader.setMat4Uniform("view",view);
        el->shader.setVec3Uniform("objectColor",{1.0f,0.5f,0.31f});
        el->shader.setVec3Uniform("lightColor",{1.0f,1.0f,1.0});
        el->shader.setVec3Uniform("lightPos",glm::vec3(1.2,1.0,0.0));
        el->draw();
      }
    }