#pragma once

#include <glm/common.hpp>

namespace acgm
{
  //! Light  - abstract base class for scene lights
  class Light
  {
  public:
    explicit Light(const float &intensity);
    virtual ~Light() = default;

    virtual glm::vec3 GetDirectionToLight(const glm::vec3 &point) const = 0;
    virtual float GetIntensityAt(const glm::vec3 &point) const = 0;
    virtual float GetDistanceToLight(const glm::vec3 &point) const = 0;

  protected:
    float intensity_;
  };
}
