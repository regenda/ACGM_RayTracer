#include <ACGM_RayTracer_lib/Light.h>

acgm::Light::Light(
  const float &intensity,
  const glm::vec3 &position)
  : intensity_(intensity)
  , position_(position)
{
}

const glm::vec3 &acgm::Light::Position() const
{
  return position_;
}
