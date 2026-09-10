#include "../include/renderable.h"

void Renderable::draw() {
    if(texture)
      texture->bind(); 
    mesh.bindVAO();
    if(mesh.indices.size() > 0) 
    glDrawElements(GL_TRIANGLES,mesh.indices.size(),GL_UNSIGNED_INT,0);
    else 
      glDrawArrays(GL_TRIANGLES,0,36);
    glBindVertexArray(0);
  }


  Renderable::Renderable(Shader& shader, Mesh& mesh, Material* material, Texture* texture,glm::vec3 * transform, glm::vec3* dimension)
    : shader(shader), mesh(mesh), material(material), texture(texture), transform(transform), dimension(dimension) 
  {

  };