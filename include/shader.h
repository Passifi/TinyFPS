#pragma once
#include <string>
#include <map>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <variant>
#include <array>
#include <assert.h>
#include "../include/glm/glm.hpp"
#include "../include/fileIO.h"
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
  std::map<std::string, unsigned int> uniformLocations; 
  void createProgram(const std::string& vertSource,const std::string& fragSource); 
  void link(const VertexShader& vert,const FragmentShader& frag); 
  void use() const; 
  void setFloatUniform(const std::string& name,const float value);
  void setVec4Uniform(const std::string& name, const std::array<float,4>& vec); 
  void setMat4Uniform(const std::string&name, const glm::mat4 mat4);
  void setVec3Uniform(const std::string& name, const glm::vec3 vec3);
  void registerUniform(const std::string& name);
};

enum ShaderType {
    VertexType,
    FragmentType
};

struct ShaderConfig {
    ShaderType type;
    std::string name;
    std::string source;
};
struct ShaderProgramConfig {
    std::string name;
    std::string fragmentShaderName;
    std::string vertexShaderName;
    std::vector<std::string> uniforms;
};

class ShaderHandler {
    public:
    std::map<std::string,Shader> createShaders(std::vector<ShaderConfig> shaderConfig, std::vector<ShaderProgramConfig> shaderProgramConfig); 

};


