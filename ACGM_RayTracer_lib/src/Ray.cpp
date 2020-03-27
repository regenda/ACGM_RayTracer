#include <ACGM_RayTracer_lib/Ray.h>


acgm::Ray::Ray(
  glm::vec3 origin,
  glm::vec3 direction)
  : origin_(origin)
  , direction_(direction)
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

const float acgm::Ray::bias_ = 0.001f;

const float acgm::Ray::getBias() const
{
  return bias_;
}
