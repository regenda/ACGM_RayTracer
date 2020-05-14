#pragma once

#include <glm/common.hpp>
#include <ACGM_RayTracer_lib/Model.h>

namespace acgm
{
  //! Triangle representation
  class Triangle: public Model
  {
  public:
    Triangle(glm::vec3 *vertices, const cogs::Color3f &color);
    Triangle(glm::vec3 *vertices);
    Triangle(glm::vec3 *vertices, glm::vec3 *normals);
    virtual Hit Intersect(const acgm::Ray &ray) const override;

    glm::vec3 *getVertices();
    glm::vec3 *getNormals();

  private:
    glm::vec3 vertices_[3];
    glm::vec3 normals_[3];

    bool smooth_;
  };
}
