#include <ACGM_RayTracer_lib/Ray.h>


acgm::Ray::Ray(
  glm::vec3 origin,
  glm::vec3 direction,
  float bias)
  : origin_(origin)
  , direction_(direction)
  , bias_(bias)
{
}

const glm::vec3 acgm::Ray::getOrigin() const
{
  return origin_;
}

const glm::vec3 acgm::Ray::getDirection() const
{
  return direction_;
}

const float acgm::Ray::getBias() const
{
  return bias_;
}
