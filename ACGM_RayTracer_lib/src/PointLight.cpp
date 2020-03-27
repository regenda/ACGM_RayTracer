#include <ACGM_RayTracer_lib/PointLight.h>

#include <glm/glm.hpp>

acgm::PointLight::PointLight(
  const float &range,
  const float &intensity,
  const glm::vec3 &position,
  const float &attL,
  const float &attQ)
  : Light(intensity, position)
  , range_(range)
  , attL_(attL)
  , attQ_(attQ)
{
}

glm::vec3 acgm::PointLight::GetDirectionToLight(const glm::vec3 &point) const
{
  return glm::normalize(Position() - point);
};

float acgm::PointLight::GetIntensityAt(const glm::vec3 &point) const
{
  // Calculate distance from light source position the point, for which the light intensity is calculated
  float distance = glm::distance(Position(), point);
  // Calculate linear attenuation
  float l = range_ / (range_ + distance * attL_);
  // Calculate quadratic attenuation
  float range_2 = range_ * range_;
  float q = range_2 / (range_2 + distance * distance * attQ_);

  // Combine linear and quadratic for final intensity
  return l * q * intensity_;
}
