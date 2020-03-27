#include <ACGM_RayTracer_lib/DirectionalLight.h>

acgm::DirectionalLight::DirectionalLight(
  const glm::vec3 &direction,
  const float &intensity,
  const glm::vec3 &position)
  : Light(intensity, position)
  , direction_(direction)
{
}

glm::vec3 acgm::DirectionalLight::GetDirectionToLight(const glm::vec3 &point) const
{
  return -direction_;
};

float acgm::DirectionalLight::GetIntensityAt(const glm::vec3 &point) const
{
  return intensity_;
}
