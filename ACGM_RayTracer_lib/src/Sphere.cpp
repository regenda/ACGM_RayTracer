#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>
#include <ACGM_RayTracer_lib/Sphere.h>



acgm::Sphere::Sphere(const glm::vec3 &position, float radius)
  : position_(position), radius_(radius)
{
}

acgm::Hit acgm::Sphere::Intersect(const acgm::Ray &ray) const
{
  Hit result;
  result.t = std::nullopt;

  glm::vec3 oo = ray.getOrigin() - position_;

  float A = glm::dot(ray.getDirection(), ray.getDirection());
  float B = -2.0f * glm::dot(oo, ray.getDirection());
  float C = glm::dot(oo, oo) - radius_ * radius_;
  float D = B * B - 4.0f * A * C;

  if (D < 0)
  {
    return result;
  }

  float sD = glm::sqrt(D);

  float t1 = 0.5f * (B + sD) / A;
  if (t1 < ray.getBias())
  {
    t1 = INFINITY;
  }

  float t2 = 0.5f * (B - sD) / A;
  if (t2 < ray.getBias())
  {
    t2 = INFINITY;
  }

  const float t = glm::min(t1, t2);
  if (glm::isinf(t) || t < 0.0f)
  {
    return result;
  }

  result.t = t;
  result.normal = glm::normalize(ray.getOrigin() + t * ray.getDirection() - position_);
  return result;
}

const glm::vec3 &acgm::Sphere::GetPosition() const
{
  return position_;
}

float acgm::Sphere::GetRadius() const
{
  return radius_;
}

