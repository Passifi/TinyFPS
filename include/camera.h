#pragma once
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

struct Camera {
  glm::vec3 position; 
  glm::vec3 target; 
  glm::vec3 direction;
  glm::vec3 right;
  glm::vec3 up;
  glm::mat4 view;
  bool isDirty = false;
  Camera() {
    position = {0.0f,0.0f,3.0f};
    target = {0.0f,0.0f,-1.0f};
    up = {0.0f,1.0f,0.0};
    view = glm::lookAt(position,position+target,up);
  }
  Camera(glm::vec3 position, glm::vec3 target, glm::vec3 direction) : position(position), target(target), direction(direction) {
    right = glm::normalize(glm::cross(glm::vec3(0.0f,1.0,0.0f),direction));
    up = glm::cross(direction,right);
    view = glm::lookAt(position,target,up);

  }
  void moveCameraY(float value) {
    position += target*value;
    isDirty = true;
  }
  void moveCameraX(float value) {
    position += glm::normalize(glm::cross(target,up))*value;
    isDirty = true;
  }
  void rotate(glm::vec3 offset);
  glm::mat4 getView() {
    if(isDirty) {
        view = glm::lookAt(position,position+target,up);
    }
    return view;
  }

};