#pragma once

#include <ACGM_RayTracer_lib/Light.h>

namespace acgm
{
  //! PointLight representation
  class PointLight : public Light
  {
  public:
    PointLight(const float &range, const float &intensity, const glm::vec3 &position, const float &attL, const float &attQ);

    virtual glm::vec3 GetDirectionToLight(const glm::vec3 &point) const override;

    virtual float GetIntensityAt(const glm::vec3 &point) const override;

    virtual float GetDistanceToLight(const glm::vec3 &point) const override;

  protected:
    float range_;
    float attL_;
    float attQ_;
    glm::vec3 position_;
  };
}
