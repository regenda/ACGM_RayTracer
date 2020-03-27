#include <ACGM_RayTracer_lib/SpotLight.h>

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

acgm::SpotLight::SpotLight(
  const float &range,
  const float &intensity,
  const glm::vec3 &position,
  const float &attL,
  const float &attQ,
  const glm::vec3 &direction,
  const float &cutoffAngle,
  const float &exponent)
  : PointLight(range, intensity, position, attL, attQ)
  , direction_(direction)
  , cutoffAngle_(cutoffAngle)
  , exponent_(exponent)
{
}

float acgm::SpotLight::GetIntensityAt(const glm::vec3 &point) const
{
  // Get angle between light direction and direction from light position to the point
  float alfa = glm::angle(glm::normalize(direction_), -GetDirectionToLight(point));

  // If angle is larger than cutoff, intensity is zero
  float cutoff = glm::radians(cutoffAngle_);
  if (alfa > cutoff)
  {
    return 0.f;
  }

  // Calculate an intensity decay
  float decay = 1 - glm::pow(alfa / cutoff, exponent_);

  return decay * PointLight::GetIntensityAt(point);
}
