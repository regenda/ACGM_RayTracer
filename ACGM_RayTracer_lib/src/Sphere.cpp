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

  float bias = ray.getBias();
  glm::vec3 origin = ray.getOrigin(), dir = ray.getDirection();

  glm::vec3 oo = origin - position_;

  float A = glm::dot(dir, dir);
  float B = -2.0f * glm::dot(oo, dir);
  float C = glm::dot(oo, oo) - radius_ * radius_;
  float D = B * B - 4.0f * A * C;

  if (D < 0)
  {
    return result;
  }

  float sD = glm::sqrt(D);

  float t1 = 0.5f * (B + sD) / A;
  if (t1 < bias)
  {
    t1 = INFINITY;
  }

  float t2 = 0.5f * (B - sD) / A;
  if (t2 < bias)
  {
    t2 = INFINITY;
  }

  const float t = glm::min(t1, t2);
  if (glm::isinf(t) || t < 0.0f)
  {
    return result;
  }

  result.t = t;
  result.normal = glm::normalize(origin + t * dir - position_);
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

