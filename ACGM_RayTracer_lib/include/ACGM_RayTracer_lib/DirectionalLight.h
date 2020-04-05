#pragma once

#include <ACGM_RayTracer_lib/Light.h>

namespace acgm
{
  //! DirectionalLight representation
  class DirectionalLight : public Light
  {
  public:
    DirectionalLight(const glm::vec3 &direction, const float &intensity);

    virtual glm::vec3 GetDirectionToLight(const glm::vec3 &point) const override;

    virtual float GetIntensityAt(const glm::vec3 &point) const override;

    virtual float GetDistanceToLight(const glm::vec3 &point) const override;

  private:
    glm::vec3 direction_;
  };
}
