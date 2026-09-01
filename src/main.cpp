#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/fileIO.h"
#define color(c) c.red,c.green,c.blue,c.alpha

struct VertexDataInfo {
  unsigned int stride;
  std::vector<int> dataPointWidths;

};

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
  std::vector<float> vertices;
  std::vector<int> indices;
  VertexDataInfo vertexDataInfo;
  unsigned int vao;
  unsigned int vbo;
  void intialize() {
    glGenBuffers(1,&vbo);
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo); 
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*vertices.size(),vertices.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(0,vertexDataInfo.stride,GL_FLOAT,GL_FALSE,vertexDataInfo.stride*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
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
    }

  }
};

struct Shader {
  unsigned int id;
  void link(VertexShader vert, FragmentShader frag) {
      id = glCreateProgram();
      glAttachShader(id,vert.id);
      glAttachShader(id,frag.id);
      glLinkProgram(id);
      glDeleteShader(vert.id);
      glDeleteShader(frag.id);
  }
  void use() {
    glUseProgram(id);
  }
};
GraphicsConfig defaultGraphicsConfig {800,600};

struct Rendereable {
  Shader shader;
  Mesh mesh;
  void draw() {
    shader.use();
    glBindVertexArray(mesh.vao); 
    glDrawArrays(GL_TRIANGLES,0,3);
  }
};

class Renderer {
  std::vector<Mesh> mesh;
};


void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int main(void) {
  Color backgroundColor{0.8,0.3,0.3,1.0};
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
  Mesh mesh; 
  mesh.vertices = {
    -.5,-0.5f,0.0f,
    .5,-0.5f,0.0f,
    .0,.5f,0.0f,
  };
  FragmentShader fragShader;
  VertexShader vertShader;
  Shader shader;
  
    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0"; 
    std::cout << fragStc << std::endl;
    std::cout << vertSrc << std::endl;
    fragShader.compile(fragStc.c_str()) ;
    vertShader.compile(vertSrc.c_str()); 
    shader.link(vertShader,fragShader);
    Rendereable renderObject{shader,mesh};
    mesh.vertexDataInfo.stride = 3;
    mesh.intialize();
   
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
