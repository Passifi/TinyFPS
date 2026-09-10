#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "camera.h"
#include "renderable.h"

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
  unsigned int screenWidth = 800;
  unsigned int screenHeight = 600;
  float fov = 45.0f;
  RendererPerspective perspectiveState = Perspective;
  public:
    Camera camera;
    void setFOV(float fov);
    void modifyFOV(float offset);
    void toggleProjectionState(); 
    void setViewport(); 
    void setScreenDimensions(unsigned int width, unsigned int height); 
    void setViewport(float fov,float width,float height);
    void initialize();
    void addRenderable(Renderable*  renderable);
    void render();    
};

