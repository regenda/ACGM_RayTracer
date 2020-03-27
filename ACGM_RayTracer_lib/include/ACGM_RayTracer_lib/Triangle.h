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
    virtual Hit Intersect(const acgm::Ray &ray) const override;

    glm::vec3 *getVertices();

  private:
    glm::vec3 vertices_[3];
  };
}
