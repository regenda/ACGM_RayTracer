#pragma once

#include <ACGM_RayTracer_lib/PointLight.h>

namespace acgm
{
  //! SpotLight representation
  class SpotLight : public PointLight
  {
  public:
    SpotLight(const float &range, const float &intensity, const glm::vec3 &position, const float &attL, const float &attQ, const glm::vec3 &direction, const float &cutoffAngle, const float &exponent);

    virtual float GetIntensityAt(const glm::vec3 &point) const override;

  private:
    glm::vec3 direction_;
    float cutoffAngle_;
    float exponent_;
  };
}
