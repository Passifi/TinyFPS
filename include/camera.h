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
  float pitch,yaw; 
  bool isDirty = false;
  Camera() {
    pitch = yaw = 0.0f;
    updateTarget();
    std::cout << target.x << target.y << target.z << std::endl;
    position = {0.0f,0.0f,3.0f};
    up = {0.0f,1.0f,0.0};
    view = glm::lookAt(position,position+target,up);
  }
  Camera(glm::vec3 position, glm::vec3 target, glm::vec3 direction) : position(position), target(target), direction(direction) {
    right = glm::normalize(glm::cross(glm::vec3(0.0f,1.0,0.0f),direction));
    up = glm::cross(direction,right);
    view = glm::lookAt(position,target,up);

  }

  void updateTarget() {
    target.x = cos(glm::radians(yaw)*cos(glm::radians(pitch)));
    target.y = sin(glm::radians(pitch));
    target.z = sin(glm::radians(yaw)*cos(glm::radians(pitch)));
    target = glm::normalize(target);
  }
  void setRotation(float offsetX, float offsetY) {
    yaw += offsetX; 
    pitch += offsetY;
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