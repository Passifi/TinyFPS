#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/fileIO.h"
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
struct Texture {};
struct Mesh {

  Mesh(std::vector<float> vertices,uint stride) : vertices(vertices) ,stride(stride) {}
  Mesh(std::vector<float> vertices,uint stride, std::vector<unsigned int> indices) : vertices(vertices), indices(indices),stride(stride) {}
  Mesh(std::vector<float> vertices) : vertices(vertices) {}
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  unsigned int stride; 
  unsigned int vao;
  unsigned int vbo;
  unsigned int ebo;
  void intialize() {
    if(indices.size() == 0) {
      glGenBuffers(1,&vbo);
      glGenVertexArrays(1,&vao);
      glBindVertexArray(vao);
      glBindBuffer(GL_ARRAY_BUFFER,vbo);
      glBufferData(GL_ARRAY_BUFFER,sizeof(float)*vertices.size(),vertices.data(),GL_STATIC_DRAW);
      glVertexAttribPointer(0,stride,GL_FLOAT,GL_FALSE,stride*sizeof(float),(void*)0);
      glEnableVertexAttribArray(0);
    } else {
    glGenBuffers(1,&vbo);
    glGenBuffers(1,&ebo);
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*vertices.size(),vertices.data(),GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*indices.size(),indices.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(0,stride,GL_FLOAT,GL_FALSE,stride*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    }

  }

  void bindVAO() {
    glBindVertexArray(vao);
  }
}; 
struct Model {};

struct VertexShader {
  unsigned int id; 
  void compile(const char* source) {
    id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(id,1,&source,NULL);
    glCompileShader(id);
    int success; 
    char infoLog[512];
    glGetShaderiv(id,GL_COMPILE_STATUS,&success);
    if(!success) {
      std::cout << "Couldnt' compile vertex shader" << std::endl;
      glGetShaderInfoLog(id,512,NULL,infoLog);
      std::cout << infoLog << std::endl;
      throw std::runtime_error("Couldn't compile vertex shader");
    }
  }
};
struct FragmentShader {
  unsigned int id; 
  void compile(const char* source) {
    id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(id,1,&source,NULL);
    glCompileShader(id);
   int success; 
    char infoLog[512];
    glGetShaderiv(id,GL_COMPILE_STATUS,&success);
    if(!success) {
      std::cout << "Couldnt' compile fragment shader" << std::endl;
      glGetShaderInfoLog(id,512,NULL,infoLog);
      std::cout << infoLog << std::endl;
      throw std::runtime_error("Couldn't compile Fragment shader");
    }

  }
};

struct Shader {
  unsigned int id;
  void createProgram(const std::string& vertSource,const std::string& fragSource) {
    VertexShader vertShader;
    FragmentShader fragShader;
    vertShader.compile(vertSource.c_str());
    fragShader.compile(fragSource.c_str());
    link(vertShader,fragShader);
    glDeleteShader(vertShader.id);
    glDeleteShader(fragShader.id);
  }
  void link(const VertexShader& vert,const FragmentShader& frag) {
      id = glCreateProgram();
      glAttachShader(id,vert.id);
      glAttachShader(id,frag.id);
      glLinkProgram(id);
  }
  void use() {
    glUseProgram(id);
  }
};
GraphicsConfig defaultGraphicsConfig {800,600};

struct Renderable {
  Shader& shader;
  Mesh& mesh;
  void draw() {
    shader.use();
    mesh.bindVAO();
    glDrawElements(GL_TRIANGLES,mesh.indices.size(),GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
  }
};

class Renderer {
  std::vector<Renderable> mesh;
};

 Mesh mesh({ 0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
  },3,{0,1,3,1,2,3}); 
  

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
  std::string fragStc = loadFile("./shaderSrc/basic.frag");
  std::string vertSrc = loadFile("./shaderSrc/basic.vert");
 
    Shader shader;
    shader.createProgram(vertSrc,fragStc);
    mesh.intialize();
    Renderable renderObject{shader,mesh};
   
    while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClearColor(color(backgroundColor));
    glClear(GL_COLOR_BUFFER_BIT);
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
