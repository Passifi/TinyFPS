#include "../include/camera.h"
Camera::Camera() {
    pitch = yaw = 0.0f;
    updateTarget();
    position = {0.0f,0.0f,3.0f};
    up = {0.0f,1.0f,0.0};
    view = glm::lookAt(position,position+target,up);
  }

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 direction) : position(position), target(target), direction(direction) {
    right = glm::normalize(glm::cross(glm::vec3(0.0f,1.0,0.0f),direction));
    up = glm::cross(direction,right);
    view = glm::lookAt(position,target,up);

  }

  void Camera::updateTarget() {
    target.x = cos(glm::radians(yaw)*cos(glm::radians(pitch)));
    target.y = sin(glm::radians(pitch));
    target.z = sin(glm::radians(yaw)*cos(glm::radians(pitch)));
    target = glm::normalize(target);
  }
  void Camera::setRotation(float offsetX, float offsetY) {
    yaw += offsetX; 
    pitch += offsetY; 
    if(pitch > 89.0f) {
      pitch = 89.0f;
    }
    else if (pitch < -89.0f) {
      pitch = -89.0f;
    }
  }
  void Camera::moveCameraY(float value) {
    position += target*value;
    isDirty = true;
  }
  void Camera::moveCameraX(float value) {
    position += glm::normalize(glm::cross(target,up))*value;
    isDirty = true;
  }
  void Camera::setPosition(glm::vec3 newPosition) {
    position = newPosition;
  }

  glm::mat4 Camera::getView() {
    if(isDirty) {
        view = glm::lookAt(position,position+target,up);
    }
    return view;
  }


