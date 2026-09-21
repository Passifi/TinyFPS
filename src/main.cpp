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

class TimeKeeper {

  float lastTick;
  float delta;
  public:
  TimeKeeper() {
    lastTick = glfwGetTime();
  }
  void update() {
    auto currentTick = glfwGetTime();
    delta = currentTick-lastTick;
    lastTick = currentTick;
  }

  float getDelta() {
    return delta;
  }
};

GLFWwindow* initializeGLFW() {
  GLFWwindow* window = nullptr;   
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
auto cubeMeshData = {

    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
 
Mesh mesh(
  cubeMeshData
);
Mesh lightSource(
  cubeMeshData
);
  std::map<std::string, Shader> createShaders() {
  ShaderHandler shaderHandler; 
  ShaderConfig vertexConfig; 
  vertexConfig.name = "light vert";
  vertexConfig.type = ShaderType::VertexType;
  vertexConfig.source = "./shaderSrc/lighting.vert";
  ShaderConfig fragmentConfig; 
  fragmentConfig.name = "light frag";
  fragmentConfig.type = ShaderType::FragmentType;
  fragmentConfig.source = "./shaderSrc/lighting.frag";
  ShaderConfig srcVertConfig{ShaderType::VertexType,"srclight vert","./shaderSrc/lightSrc.vert"};
  ShaderConfig srcFragmentConfig{ShaderType::FragmentType,"srclight frag","./shaderSrc/lightSrc.frag"};
  ShaderProgramConfig lighting;
  ShaderProgramConfig lightSource{"lighting Source","srclight vert","srclight frag",{"model","projection","view"}};
  lighting.fragmentShaderName = "light frag";
  lighting.vertexShaderName = "light vert";
  lighting.name = "lighting";
  lighting.uniforms = {"model","projection","view","lightPos","objectColor","lightColor"};
  return shaderHandler.createShaders({vertexConfig,fragmentConfig,srcVertConfig,srcFragmentConfig},{lighting,lightSource});
}

TimeKeeper timeKeeper;

void processInput(GLFWwindow *window);
float deltaTime =0.0f;
Renderer renderer;
glm::vec3*  lightPos = nullptr;
int main(void) {
  auto window = initializeGLFW(); 
  auto imageData = loadImageData("./assets/container.jpg");
  Texture texture(imageData);
  auto shaders = createShaders(); 
  mesh.intialize();
  renderer.initialize();
  Material stdMaterial;
  std::vector<glm::vec3*> transforms; 
  glm::vec3 scale(1.0f,1.0f,1.0f); 
  std::vector<glm::vec3*> scales{&scale};

  auto currentTransform = new glm::vec3(0.0,0.0,0.0);
  transforms.push_back(currentTransform);
  Renderable* renderable = new Renderable(shaders["lighting"],mesh,&stdMaterial,&texture,transforms.back(),scales.back());
  
  renderer.addRenderable(renderable); 
  transforms.push_back(new glm::vec3(-1.2,2.0,0.0));
  scales.push_back(new glm::vec3{0.2,0.2,0.2});
  renderable->lightSource = transforms.back();
  lightPos = transforms.back();
  renderable = new Renderable(shaders["lighting Source"],mesh,&stdMaterial,&texture,transforms.back(),scales.back());
  renderer.addRenderable(renderable); 
  
  while (!glfwWindowShouldClose(window)) {
      timeKeeper.update(); 
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
  float cameraSpeed = cameraSpeedBase*timeKeeper.getDelta();
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
  if(glfwGetKey(window,GLFW_KEY_E) == GLFW_PRESS) {
    lightPos->x = renderer.camera.position.x + renderer.camera.direction.x;
    lightPos->y = renderer.camera.position.y+ renderer.camera.direction.y;
    lightPos->z = renderer.camera.position.z+ renderer.camera.direction.z;
  }
  if(glfwGetKey(window,GLFW_KEY_X) == GLFW_PRESS) {
    //renderer.toggleProjectionState();
    //renderer.setViewport(); 
    // viewport switching breaks the display. 
    // This is due to orthographic projection, but for whatever reason goign tback to perspective projection doesn't fix it,
    // so currently this break the app
  }

}

void mouseCallback([[maybe_unused]] GLFWwindow* window, double xPosition, double yPosition) {
   mouse.updateMouse(xPosition,yPosition);
  renderer.camera.setRotation(mouse.getOffsetX(),mouse.getOffsetY());
  renderer.camera.updateTarget();
}
void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  renderer.setScreenDimensions(width,height);
  renderer.setViewport();
}


void scroll_callback([[maybe_unused]] GLFWwindow* window,[[maybe_unused]] double xoffset,double yoffset)
{
  renderer.modifyFOV(yoffset);
  renderer.setViewport();
}