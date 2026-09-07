#pragma once 
#include <glad/glad.h>
struct TextureImage {
  int width,height,nrChannels;
  unsigned char* data;
};
struct Texture {
  unsigned int id;
  Texture(TextureImage img) {
    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_2D,id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img.width,img.height,0,GL_RGB,GL_UNSIGNED_BYTE,img.data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  void bind() {
    glBindTexture(GL_TEXTURE_2D,id);
  }
   
};