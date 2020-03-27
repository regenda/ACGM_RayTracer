#include <ACGM_RayTracer_lib/Plane.h>
#include <glm/glm.hpp>


acgm::Plane::Plane(
  glm::vec3 point,
  glm::vec3 normal,
  const cogs::Color3f &color)
  : Model(color)
  , point_(point)
  , normal_(normal)
{
}

acgm::Plane::Plane(
  glm::vec3 point,
  glm::vec3 normal)
  : point_(point)
  , normal_(normal)
{
}

glm::vec3 acgm::Plane::getPoint()
{
  return point_;
}

glm::vec3 acgm::Plane::getNormal()
{
  return normal_;
}

acgm::Hit acgm::Plane::Intersect(const acgm::Ray &ray) const
{
  Hit result;
  result.t = std::nullopt;
  result.normal = normal_;

  glm::vec3 direction = ray.getDirection();

  float denominator = glm::dot(direction, normal_);

  float epsilon = ray.getBias();
  if (glm::abs(denominator) <= epsilon)
  {
    return result;
  }

  glm::vec3 origin = ray.getOrigin();
  glm::vec3 q_p = point_ - origin;

  float t = glm::dot(q_p, normal_) / denominator;

  if (t <= epsilon)
  {
    return result;
  }

  result.t = t;
  return result;
}
