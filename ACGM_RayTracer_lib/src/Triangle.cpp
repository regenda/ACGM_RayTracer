#include <ACGM_RayTracer_lib/Triangle.h>
#include <glm/glm.hpp>

#include<iostream>

acgm::Triangle::Triangle(glm::vec3 *vertices, const cogs::Color3f &color)
  : Model(color)
{
  vertices_[0] = vertices[0];
  vertices_[1] = vertices[1];
  vertices_[2] = vertices[2];
}

acgm::Triangle::Triangle(glm::vec3 *vertices)
{
  vertices_[0] = vertices[0];
  vertices_[1] = vertices[1];
  vertices_[2] = vertices[2];
}

glm::vec3 *acgm::Triangle::getVertices()
{
  return vertices_;
}

acgm::Hit acgm::Triangle::Intersect(const acgm::Ray &ray) const
{
  Hit result;
  result.t = std::nullopt;

  glm::vec3 v_01 = vertices_[1] - vertices_[0];
  glm::vec3 v_02 = vertices_[2] - vertices_[0];

  // triangle normal
  glm::vec3 normal = glm::normalize(glm::cross(v_01, v_02));
  result.normal = normal;

  glm::vec3 direction = ray.getDirection();

  float denominator = glm::dot(direction, normal);

  float epsilon = ray.getBias();
  if (glm::abs(denominator) <= epsilon)
  {
    return result;
  }

  glm::vec3 origin = ray.getOrigin();
  glm::vec3 q_p = vertices_[0] - origin;

  float t = glm::dot(q_p, normal) / denominator;

  if (t <= epsilon)
  {
    return result;
  }

  glm::vec3 intersectPoint = origin + t * direction;

  // inside-outside test

  // edges between vertices
  glm::vec3 edge1 = v_01;
  glm::vec3 edge2 = vertices_[2] - vertices_[1];
  glm::vec3 edge3 = vertices_[0] - vertices_[2];

  // edges between vertices and intersectPoint
  glm::vec3 pedge1 = intersectPoint - vertices_[0];
  glm::vec3 pedge2 = intersectPoint - vertices_[1];
  glm::vec3 pedge3 = intersectPoint - vertices_[2];

  if (glm::dot(glm::cross(edge1, pedge1), normal) < 0 ||
    glm::dot(glm::cross(edge2, pedge2), normal) < 0 ||
    glm::dot(glm::cross(edge3, pedge3), normal) < 0)
  {
    return result;
  }
  result.t = t;
  return result;
}
