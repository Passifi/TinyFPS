#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/shader.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include <cmath>
#define color(c) c.red,c.green,c.blue,c.alpha

struct Color {
  float red;
  float green;
  float blue;
  float alpha;
};

struct GraphicsConfig {
  int screenWidth;
  int screenHeigth;
};

GraphicsConfig defaultGraphicsConfig {800,600};

class Camera {};
class Input {};

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
  void draw() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model,*transform);
    shader.setMat4Uniform("model",model); 
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
class Renderer {
  std::vector<Renderable*> renderables;
  glm::mat4 camera;
  glm::mat4 projection;
  uint screenWidth;
  uint screentHeight;
  public:

    GLFWwindow* window = nullptr;

    int intialize() {
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
      return -1;
    }
      glfwMakeContextCurrent(window);
      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
      }
      glViewport(0, 0, defaultGraphicsConfig.screenWidth, defaultGraphicsConfig.screenHeigth);
      glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
      return 0;
    }
    void addRenderable(Renderable*  renderable) {
      renderables.push_back(renderable); 
    }
    void render() {
      glm::mat4 projection = glm::mat4(1.0f);
      glm::mat4 model = glm::mat4(1.0f);
      glm::mat4 view = glm::mat4(1.0f);
      model = glm::rotate(model,glm::radians(-55.f),glm::vec3(1.0f,0.0f,0.0f));
      view = glm::translate(view,glm::vec3(0.0f,0.0f,-3.0f));
      projection = glm::perspective(glm::radians(45.f),800.0f/600.0f,0.1f,100.0f);
      for(auto&el : renderables) {
        el->shader.setMat4Uniform("projection",projection); 
        el->shader.setMat4Uniform("view",view);
        el->draw();
      }
    }
};

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
  return shaderHandler.createShaders({vertexConfig,fragmentConfig},{ShaderProgramConfig});
}
void processInput(GLFWwindow *window);
int main(void) {
  Color backgroundColor{0.3f,0.3f,1.0f,1.0f};
  Renderer renderer;
  renderer.intialize();  
  auto window = renderer.window; 
    auto imageData = loadImageData("./assets/container.jpg");
    Texture texture(imageData);
    auto shaders = createShaders(); 
    mesh.intialize();
    shaders["basic"].registerUniform("transform");
    shaders["basic"].registerUniform("projection");
    shaders["basic"].registerUniform("model");
    shaders["basic"].registerUniform("view");
    Renderable renderable{shaders["basic"],mesh,&texture};
    renderable.transform = new glm::vec3(0.0f,0.0f,0.0f);
    renderer.addRenderable(&renderable); 
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
      processInput(window);
      glClearColor(color(backgroundColor));
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      renderable.transform->x +=0.01;
      renderer.render(); 
      glfwSwapBuffers(window);
      glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(window, true);
  }
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}