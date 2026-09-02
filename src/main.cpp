#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/shader.h"
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
class Camera {};
class Input {};
struct TextureImage {
  int width,height,nrChannels;
  unsigned char* data;
};
TextureImage loadImageData(char* source) {
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
struct Mesh {
  Mesh(std::vector<float> vertices,uint stride) : vertices(vertices) ,stride(stride) {}
  Mesh(std::vector<float> vertices,uint stride, std::vector<unsigned int> indices) : vertices(vertices), indices(indices),stride(stride) {}
  Mesh(std::vector<float> vertices,uint stride, std::vector<unsigned int> indices,std::vector<unsigned int> attribInfo) : vertices(vertices), indices(indices),stride(stride), vertexAttribInfo(attribInfo) {}
  Mesh(std::vector<float> vertices) : vertices(vertices) {}
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  std::vector<unsigned int> vertexAttribInfo;
  unsigned int stride; 
  unsigned int vao;
  unsigned int vbo;
  unsigned int ebo;
  void setVertexAttributes() {
    if(vertexAttribInfo.size() == 0) {
     glVertexAttribPointer(0,stride,GL_FLOAT,GL_FALSE,stride*sizeof(float),(void*)0);
      glEnableVertexAttribArray(0);
    }
    else {
      int attributeArrayIndex = 0;
      int currentLength = 0;
      for(auto& length : vertexAttribInfo) {
        glVertexAttribPointer(attributeArrayIndex,length,GL_FLOAT,GL_FALSE, stride*sizeof(float),(void*)(currentLength*sizeof(float)));
        glEnableVertexAttribArray(attributeArrayIndex++);
        currentLength += length;
      }
   }
  } 
  void intialize() {
      glGenBuffers(1,&vbo);
      glGenBuffers(1,&ebo);
      glGenVertexArrays(1,&vao);
      glBindVertexArray(vao);
      glBindBuffer(GL_ARRAY_BUFFER,vbo);
      glBufferData(GL_ARRAY_BUFFER,sizeof(float)*vertices.size(),vertices.data(),GL_STATIC_DRAW);
      glVertexAttribPointer(0,stride,GL_FLOAT,GL_FALSE,stride*sizeof(float),(void*)0);
      glEnableVertexAttribArray(0);
      if(indices.size() != 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*indices.size(),indices.data(),GL_STATIC_DRAW);
   
      }
    setVertexAttributes();
  }

  void bindVAO() {
    glBindVertexArray(vao);
  }
}; 
struct Model {};

GraphicsConfig defaultGraphicsConfig {800,600};

struct Renderable {
  Shader& shader;
  Mesh& mesh;
  Texture* texture;
  void draw() {
    shader.use();
    if(texture)
    texture->bind(); 
    mesh.bindVAO();
    glDrawElements(GL_TRIANGLES,mesh.indices.size(),GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
  }
};

class Renderer {
  std::vector<Renderable> mesh;
};

 Mesh mesh(
 {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
}     
  ,8,{0,1,2,3,0},{3,3,2}); 
  
std::map<std::string, Shader> createShaders() {
  ShaderHandler shaderHandler; 
  ShaderConfig vertexConfig; 
  vertexConfig.name = "basic vert";
  vertexConfig.type = ShaderType::Vertex;
  vertexConfig.source = "./shaderSrc/basic.vert";
  ShaderConfig fragmentConfig; 
  fragmentConfig.name = "basic frag";
  fragmentConfig.type = ShaderType::Fragment;
  fragmentConfig.source = "./shaderSrc/basic.frag";
  ShaderProgramConfig ShaderProgramConfig; 
  ShaderProgramConfig.fragmentShaderName = "basic frag";
  ShaderProgramConfig.vertexShaderName = "basic vert";
  ShaderProgramConfig.name = "basic";
  return shaderHandler.createShaders({vertexConfig,fragmentConfig},{ShaderProgramConfig});
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int main(void) {
  Color backgroundColor{0.2,0.3,0.3,1.0};
  // intialize Glfws
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Ccreate Window
 GLFWwindow *window = glfwCreateWindow(defaultGraphicsConfig.screenWidth, defaultGraphicsConfig.screenHeigth, "LearnOpenGL", NULL, NULL);
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
    auto imageData = loadImageData("./assets/container.jpg");
    std::cout << imageData.width << std::endl; 
    Texture texture(imageData);

    auto shaders = createShaders(); 
    mesh.intialize();
    shaders["basic"].registerUniform("timeColor");
    Renderable renderObject{shaders["basic"],mesh,&texture};
    while (!glfwWindowShouldClose(window)) {
      processInput(window);
      glClearColor(color(backgroundColor));
      glClear(GL_COLOR_BUFFER_BIT);
      float timeValue = glfwGetTime();
      float greenValue = (std::sin(timeValue) / 2.0f) + 0.5f;
      shaders["basic"].setVec4Uniform("timeColor",{greenValue,greenValue,greenValue,0.2f});
      renderObject.draw();
      glfwSwapBuffers(window);
      glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(window, true);
  }
}
