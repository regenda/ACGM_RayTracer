#include <ACGM_RayTracer_lib/Triangle.h>
#include <glm/glm.hpp>

#include<iostream>

acgm::Triangle::Triangle(glm::vec3 *vertices, const cogs::Color3f &color)
  : Model(color)
{
  vertices_[0] = vertices[0];
  vertices_[1] = vertices[1];
  vertices_[2] = vertices[2];

  smooth_ = false;
}

acgm::Triangle::Triangle(glm::vec3 *vertices)
{
  vertices_[0] = vertices[0];
  vertices_[1] = vertices[1];
  vertices_[2] = vertices[2];

  smooth_ = false;
}

acgm::Triangle::Triangle(glm::vec3 *vertices, glm::vec3 *normals)
{
  vertices_[0] = vertices[0];
  vertices_[1] = vertices[1];
  vertices_[2] = vertices[2];

  normals_[0] = normals[0];
  normals_[1] = normals[1];
  normals_[2] = normals[2];

  smooth_ = true;
}

glm::vec3 *acgm::Triangle::getVertices()
{
  return vertices_;
}

glm::vec3 *acgm::Triangle::getNormals()
{
  return normals_;
}

acgm::Hit acgm::Triangle::Intersect(const acgm::Ray &ray) const
{
  Hit result;
  result.t = std::nullopt;

  glm::vec3 v_01 = vertices_[1] - vertices_[0];
  glm::vec3 v_02 = vertices_[2] - vertices_[0];

  // triangle non-normalized normal
  glm::vec3 normal = glm::cross(v_01, v_02);

  // 2 x triangle area for calculating barycentric coordinates
  float area2;
  if (smooth_)
  {
    area2 = glm::length(normal);
  }

  normal = glm::normalize(normal);
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

  glm::vec3 cros1 = glm::cross(edge1, pedge1);
  glm::vec3 cros2 = glm::cross(edge2, pedge2);
  glm::vec3 cros3 = glm::cross(edge3, pedge3);

  if (glm::dot(cros1, normal) < 0 ||
    glm::dot(cros2, normal) < 0 ||
    glm::dot(cros3, normal) < 0)
  {
    return result;
  }
  result.t = t;

  // Smooth Normals
  if (smooth_)
  {
    // Barycentric Coordinates
    float u = glm::length(cros2) / area2;
    float v = glm::length(cros3) / area2;

    result.normal = glm::normalize(u * normals_[0] + v * normals_[1] + (1 - u - v) * normals_[2]);
  }

  return result;
}
