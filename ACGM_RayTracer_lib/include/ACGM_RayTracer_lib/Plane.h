#pragma once

#include <ACGM_RayTracer_lib/Model.h>


namespace acgm
{
  //! Plane representation
  class Plane : public Model
  {
  public:
    Plane(glm::vec3 p, glm::vec3 n, const cogs::Color3f &color);
    Plane(glm::vec3 p, glm::vec3 n);

    glm::vec3 getPoint();
    glm::vec3 getNormal();

    virtual Hit Intersect(const acgm::Ray &ray) const override;

  private:
    glm::vec3 point_;
    glm::vec3 normal_;
  };
}
