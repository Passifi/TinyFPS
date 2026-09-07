#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/shader.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/mesh.h"
#include "../include/graphics.h"
#include "../include/Mouse.h"
#include "../include/texture.h"
#include <cmath>
struct GraphicsConfig {
  int screenWidth;
  int screenHeigth;
};


GraphicsConfig defaultGraphicsConfig {800,600};
#ifndef STDB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

TextureImage loadImageData(const char* source) {
  TextureImage img;
  img.data = stbi_load(source,&img.width,&img.height,&img.nrChannels,0);
  return img;
}
void mouseCallback(GLFWwindow* window, double xPosition, double yPosition);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
GLFWwindow* initializeGLFW() {
  GLFWwindow* window = nullptr;   
  Color backgroundColor{0.2,0.3,0.3,1.0};
  // intialize Glfws
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Ccreate Window
    window = glfwCreateWindow(defaultGraphicsConfig.screenWidth, defaultGraphicsConfig.screenHeigth, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
      std::cout << "Couldn't Initialize GLFW Window" << std::endl;
      glfwTerminate();
      return nullptr;
    }
      glfwMakeContextCurrent(window);
      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
      }
      glViewport(0, 0, defaultGraphicsConfig.screenWidth, defaultGraphicsConfig.screenHeigth);
      glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
      glfwSetCursorPosCallback(window,mouseCallback);
      glfwSetScrollCallback(window,scroll_callback);
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      return window;
    }
Mouse mouse; 
 Mesh mesh(
 {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
}    
  ); 
  
std::map<std::string, Shader> createShaders() {
  ShaderHandler shaderHandler; 
  ShaderConfig vertexConfig; 
  vertexConfig.name = "basic vert";
  vertexConfig.type = ShaderType::VertexType;
  vertexConfig.source = "./shaderSrc/basic.vert";
  ShaderConfig lightVert; 
  lightVert.name = "basic vert";
  lightVert.type = ShaderType::VertexType;
  lightVert.source = "./shaderSrc/lighting.vert";
  
  ShaderConfig fragmentConfig; 
  fragmentConfig.name = "basic frag";
  fragmentConfig.type = ShaderType::FragmentType;
  fragmentConfig.source = "./shaderSrc/basic.frag";
  ShaderConfig lightFragment;
  lightFragment.name = "light frag";
  lightFragment.type = ShaderType::FragmentType; 
  lightFragment.source = "./shaderSrc/lighting.frag";
  ShaderProgramConfig shaderProgramConfig; 
  shaderProgramConfig.fragmentShaderName = "basic frag";
  shaderProgramConfig.vertexShaderName = "basic vert";
  shaderProgramConfig.name = "basic";
  shaderProgramConfig.uniforms = {"mvp"};
  ShaderProgramConfig lighting; 
  lighting.fragmentShaderName = "light frag";
  lighting.vertexShaderName = "basic vert";
  lighting.name = "lighting";
  lighting.uniforms = {"mvp","objectColor","lightColor","diffuse","specular","ambient"};


  return shaderHandler.createShaders({vertexConfig,fragmentConfig,lightFragment},{lighting});
}

void processInput(GLFWwindow *window);
float deltaTime =0.0f;
Renderer renderer;
int main(void) {
  auto window = initializeGLFW(); 
  auto imageData = loadImageData("./assets/container.jpg");
  
  // load textures 
  // load shaders 
  // load materials 
  // bind to Renderables according to composition principles
 
  Texture texture(imageData);
  auto shaders = createShaders(); 
  mesh.intialize();
  renderer.initialize();
  std::vector<glm::vec3*> transforms; 
  glm::vec3 scale(0.2f,0.2f,0.2f); 
    Material stdMaterial{0.2f,0.9f,0.5f};
    for(int i =0;i < 100; i++) { 
      float rndX = -1.0f + 2.0f*(float)std::rand()/(float)RAND_MAX;
      float rndY = -1.0f + 2.0f*(float)std::rand()/RAND_MAX;
      float rndZ = -1.0f + 2.0f*(float)std::rand()/RAND_MAX;
      auto currentTransform = new glm::vec3(rndX,rndY,rndZ);
      transforms.push_back(currentTransform);

      Renderable* renderable = new Renderable(shaders["lighting"],mesh,&stdMaterial,&texture,currentTransform,&scale);
      renderer.addRenderable(renderable); 
    }


    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
      float delta = glfwGetTime()-lastTime;
      deltaTime = delta; 
      lastTime = glfwGetTime();
      processInput(window);
      renderer.render(); 
      glfwSwapBuffers(window);
      glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {

  const float cameraSpeedBase = 10.0f; 
  float cameraSpeed = cameraSpeedBase*deltaTime;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(window, true);
  }
  if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS) {
    renderer.camera.moveCameraY(cameraSpeed);
  }
  if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS) {
    renderer.camera.moveCameraY(-cameraSpeed);
  }
  if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS) {
    renderer.camera.moveCameraX(-cameraSpeed);
  }
  if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS) {
    renderer.camera.moveCameraX(cameraSpeed);
  }
  if(glfwGetKey(window,GLFW_KEY_X) == GLFW_PRESS) {
    //renderer.toggleProjectionState();
    //renderer.setViewport(); 
    // viewport switching breaks the display. 
    // This is due to orthographic projection, but for whatever reason goign tback to perspective projection doesn't fix it,
    // so currently this break the app
  }

}

void mouseCallback(GLFWwindow* window, double xPosition, double yPosition) {
   mouse.updateMouse(xPosition,yPosition);
  renderer.camera.setRotation(mouse.getOffsetX(),mouse.getOffsetY());
  renderer.camera.updateTarget();
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  renderer.setScreenDimensions(width,height);
  renderer.setViewport();
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  renderer.modifyFOV(yoffset);
  renderer.setViewport();
}