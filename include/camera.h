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
  Camera(); 
  Camera(glm::vec3 position, glm::vec3 target, glm::vec3 direction);
  void updateTarget(); 
  void setRotation(float offsetX, float offsetY); 
  void moveCameraY(float value); 
  void moveCameraX(float value); 
  void setPosition(glm::vec3 newPosition); 
  void rotateAround(glm::vec3 offset,glm::vec3 target);
  glm::mat4 getView();
};