#pragma once

#include <glm/common.hpp>

namespace acgm
{
  //! Representation of a ray
  class Ray
  {
  public:
    Ray(
      const glm::vec3 origin,
      const glm::vec3 direction);

    const glm::vec3 getOrigin() const;
    const glm::vec3 getDirection() const;
    const float getBias() const;

  private:
    glm::vec3 origin_;
    glm::vec3 direction_;
    static const float bias_;
  };
}
