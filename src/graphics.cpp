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
        // supplied by the renderer
        // set once per model 
        el->shader.setMat4Uniform("model",mvp);
        el->shader.setMat4Uniform("projection",projection);
        el->shader.setMat4Uniform("view",view);
        // supplied by material
        // still needs to be set when shader has changed or 
        // we use a different material
        el->shader.setVec3Uniform("objectColor",{el->material->color.red,el->material->color.blue,el->material->color.blue});
        el->shader.setVec3Uniform("lightColor",{1.0f,1.0f,1.0});
        el->shader.setVec3Uniform("material.ambient",el->material->ambient);
        el->shader.setVec3Uniform("material.diffuse",el->material->diffuse);
        el->shader.setVec3Uniform("material.specular",el->material->specular);
        el->shader.setFloatUniform("material.shininess",el->material->shininess);

        // supplied by renderer 
        // set when viewPos or lightPos have changed
        el->shader.setVec3Uniform("viewPos",camera.position);
        if(el->lightSource) {
          el->shader.setVec3Uniform("lightPos",*el->lightSource);
        }
        el->draw();
      }
    }