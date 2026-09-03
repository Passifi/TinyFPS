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

void Shader::setMat4Uniform(const std::string&name, const glm::mat4 mat4) {
    glUniformMatrix4fv(uniformLocations[name],1,GL_FALSE,glm::value_ptr(mat4));
}
void Shader::registerUniform(const std::string& name) {
    assert(uniformLocations.count(name) ==0);
    int locationId = glGetUniformLocation(id,name.c_str());
    uniformLocations[name]  = locationId;
}