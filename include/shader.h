#pragma once
#include <string>
#include <map>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <variant>
#include <array>

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
  void setFloatUniform(std::string name, float value) {
    glUniform1f(uniformLocations[name],value);
  }
  void setVec4Uniform(std::string name, std::array<float,4> vec) {
      glUniform4f(uniformLocations[name],vec[0],vec[1],vec[2],vec[3]);
  }
  void registerUniform(std::string name) {
    int locationId = glGetUniformLocation(id,name.c_str());
    uniformLocations[name]  = locationId;
  }
};

enum ShaderType {
    Vertex,
    Fragment
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
                case Fragment: {
                   FragmentShader fragShader;
                   auto source = loadFile(config.source); 
                   fragShader.compile(source.c_str());
                   fragShaders[config.name] = fragShader;
                   break; }
                case Vertex: {
                    VertexShader vertShader;
                    auto source = loadFile(config.source); 
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


