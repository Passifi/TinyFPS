#include "../include/shader.h"
#include "../include/glm/gtc/type_ptr.hpp"
void Shader::createProgram(const std::string& vertSource,const std::string& fragSource) {
    VertexShader vertShader;
    FragmentShader fragShader;
    vertShader.compile(vertSource.c_str());
    fragShader.compile(fragSource.c_str());
    link(vertShader,fragShader);
}
void Shader::link(const VertexShader& vert,const FragmentShader& frag) {
      id = glCreateProgram();
      std::cout << id << ", Frag: " << frag.id << ", Vert: " << vert.id << std::endl;
      glAttachShader(id,vert.id);
      glAttachShader(id,frag.id);
      glLinkProgram(id);
}
void Shader::use() const {
    glUseProgram(id);
}
void Shader::setFloatUniform(const std::string& name,const float value) {
    glUniform1f(uniformLocations[name],value);
}
void Shader::setVec4Uniform(const std::string& name,const std::array<float,4>& vec) {
      glUniform4f(uniformLocations[name],vec[0],vec[1],vec[2],vec[3]);
}


void Shader::setVec3Uniform(const std::string& name, const glm::vec3 vec3) {
    glUniform3fv(uniformLocations[name],1,&vec3[0]);
}

void Shader::setMat4Uniform(const std::string&name, const glm::mat4 mat4) {
    glUniformMatrix4fv(uniformLocations[name],1,GL_FALSE,glm::value_ptr(mat4));
}
void Shader::registerUniform(const std::string& name) {
    assert(uniformLocations.count(name) ==0);
    int locationId = glGetUniformLocation(id,name.c_str());
    uniformLocations[name]  = locationId;
}

std::map<std::string,Shader> ShaderHandler::createShaders(std::vector<ShaderConfig> shaderConfig, std::vector<ShaderProgramConfig> shaderProgramConfig) {
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
                   break; 
                }
                case VertexType: {
                    VertexShader vertShader;
                    auto source = loadFromFile(config.source); 
                    vertShader.compile(source.c_str());
                    vertexShaders[config.name] = vertShader;
                    break;
                }
            }
        }
        for(auto& el : fragShaders) {
            std::cout << el.first << std::endl;
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
            for(auto& uniform : config.uniforms) {
              shaders[config.name].registerUniform(uniform);
            }
        }
        return shaders;
    }
