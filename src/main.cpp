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
#include <cmath>

struct GraphicsConfig {
  int screenWidth;
  int screenHeigth;
};

GraphicsConfig defaultGraphicsConfig {800,600};


class Input {};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
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
      return window;
    }
 
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
  ShaderConfig fragmentConfig; 
  fragmentConfig.name = "basic frag";
  fragmentConfig.type = ShaderType::FragmentType;
  fragmentConfig.source = "./shaderSrc/basic.frag";
  ShaderProgramConfig ShaderProgramConfig; 
  ShaderProgramConfig.fragmentShaderName = "basic frag";
  ShaderProgramConfig.vertexShaderName = "basic vert";
  ShaderProgramConfig.name = "basic";
  ShaderProgramConfig.uniforms = {"mvp"};
  return shaderHandler.createShaders({vertexConfig,fragmentConfig},{ShaderProgramConfig});
}

void processInput(GLFWwindow *window);
float deltaTime =0.0f;
Renderer renderer;
int main(void) {
  Color backgroundColor{0.3f,0.3f,1.0f,1.0f};
  auto window = initializeGLFW(); 
    auto imageData = loadImageData("./assets/container.jpg");
    Texture texture(imageData);
    auto shaders = createShaders(); 
    mesh.intialize();
    renderer.initialize();
    std::vector<glm::vec3*> transforms; 
    glm::vec3 scale(0.02f,0.02f,0.02f); 
    for(int i =0;i < 100; i++) { 
      float rndX = -1.0f + 2.0f*(float)std::rand()/(float)RAND_MAX;
      float rndY = -1.0f + 2.0f*(float)std::rand()/RAND_MAX;
      float rndZ = -1.0f + 2.0f*(float)std::rand()/RAND_MAX;
      auto currentTransform = new glm::vec3(rndX,rndY,rndZ);
      transforms.push_back(currentTransform);
      Renderable* renderable = new Renderable{shaders["basic"],mesh,&texture,currentTransform,&scale};
      renderer.addRenderable(renderable); 
    }


    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
      float delta = glfwGetTime()-lastTime;
      deltaTime = delta; 
      lastTime = glfwGetTime();
      processInput(window);
     for(auto& el : transforms) {
        el->y -= 1.0f*delta;
        if(el->y < -2.0f) {
          el->x = -1.0f + 2.0f*(float)std::rand()/(float)RAND_MAX;
          el->z = -1.0f + 2.0f*(float)std::rand()/(float)RAND_MAX;
          el->y = 2.0f;
        }
      }
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


}
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}