#pragma once

#include <ACGM_RayTracer_lib/Light.h>

namespace acgm
{
  //! DirectionalLight representation
  class DirectionalLight : public Light
  {
  public:
    DirectionalLight(const glm::vec3 &direction, const float &intensity, const glm::vec3 &position = glm::vec3(0.f, FLT_MAX, 0.f));

    virtual glm::vec3 GetDirectionToLight(const glm::vec3 &point) const override;

    virtual float GetIntensityAt(const glm::vec3 &point) const override;

  private:
    glm::vec3 direction_;
  };
}
