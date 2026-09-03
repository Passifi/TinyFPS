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
};

class ShaderHandler {
    public:
    std::map<std::string,Shader> createShaders(std::vector<ShaderConfig> shaderConfig, std::vector<ShaderProgramConfig> shaderProgramConfig) {
        std::map<std::string,FragmentShader> fragShaders;
        std::map<std::string,VertexShader> vertexShaders;
        std::map<std::string,Shader> shaders;
        for(auto& config : shaderConfig) {
            switch(config.type) {
                case FragmentType: {
                   FragmentShader fragShader;
                   auto source = loadFromFile(config.source); 
                   fragShader.compile(source.c_str());
                   fragShaders[config.name] = fragShader;
                   break; }
                case VertexType: {
                    VertexShader vertShader;
                    auto source = loadFromFile(config.source); 
                    vertShader.compile(source.c_str());
                    vertexShaders[config.name] = vertShader;
                    break;
                }
            }
        }
        for(auto &config : shaderProgramConfig) {
            Shader shader;
            if(fragShaders.count(config.fragmentShaderName) == 0 || vertexShaders.count(config.vertexShaderName)==0) {
                std::cout << "Coudln't create shader of name " << config.name << std::endl; 
                continue;
            }
            auto fragShader = fragShaders[config.fragmentShaderName];
            auto vertShader = vertexShaders[config.vertexShaderName];
            shader.link(vertShader,fragShader);
            shaders[config.name] = shader;
        }
        return shaders;
    }
};


