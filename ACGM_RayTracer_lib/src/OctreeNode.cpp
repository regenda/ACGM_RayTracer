#include <ACGM_RayTracer_lib/OctreeNode.h>
#include <iostream>

unsigned int acgm::OctreeNode::LEVEL_MAX = 4;
unsigned int acgm::OctreeNode::LIMIT_MAX = 10;

acgm::OctreeNode::OctreeNode(
  glm::vec3 &center,
  glm::vec3 &size,
  unsigned int level,
  OctreeNode *parent)
  : center_(center)
  , size_(size)
  , level_(level)
  , parent_(parent)
{
  float sizeX_2 = size_.x / 2.f;
  float sizeY_2 = size_.y / 2.f;
  float sizeZ_2 = size_.z / 2.f;

  float sizeX_2plus = center_.x + sizeX_2;
  float sizeX_2minus = center_.x - sizeX_2;
  float sizeY_2plus = center_.y + sizeY_2;
  float sizeY_2minus = center_.y - sizeY_2;
  float sizeZ_2plus = center_.z + sizeZ_2;
  float sizeZ_2minus = center_.z - sizeZ_2;

  childCenter_[0] = glm::vec3(sizeX_2minus, sizeY_2plus, sizeZ_2minus);
  childCenter_[1] = glm::vec3(sizeX_2plus, sizeY_2plus, sizeZ_2minus);
  childCenter_[2] = glm::vec3(sizeX_2minus, sizeY_2minus, sizeZ_2minus);
  childCenter_[3] = glm::vec3(sizeX_2plus, sizeY_2minus, sizeZ_2minus);
  childCenter_[4] = glm::vec3(sizeX_2minus, sizeY_2plus, sizeZ_2plus);
  childCenter_[5] = glm::vec3(sizeX_2plus, sizeY_2plus, sizeZ_2plus);
  childCenter_[6] = glm::vec3(sizeX_2minus, sizeY_2minus, sizeZ_2plus);
  childCenter_[7] = glm::vec3(sizeX_2plus, sizeY_2minus, sizeZ_2plus);

  for (unsigned int i = 0; i < 8; i++)
  {
    child_[i] = NULL;
  }
}

acgm::OctreeNode::~OctreeNode()
{
  for (unsigned int i = 0; i < 8; i++)
  {
    if (child_[i])
    { delete child_[i]; }
  }
}

void acgm::OctreeNode::addTriangle(Triangle triangle)
{
  glm::vec3 halfSize = size_ / 2.f;
  if (!limitReached_ || level_ == LEVEL_MAX)
  {
    list_.push_back(triangle);

    if (list_.size() >= LIMIT_MAX && level_ < LEVEL_MAX)
    {
      limitReached_ = true;
      for (Triangle tri : list_)
      {
        glm::vec3 *vertices = tri.getVertices();

        // triangle can be stored in more than one child node if the size is too large
        for (unsigned int i = 0; i < 8; i++)
        {
          // test if triangle belongs to that sub cube
          if (triangleInsideAABB(vertices, childCenter_[i], halfSize))
          {
            if (child_[i] == NULL)
            { child_[i] = new OctreeNode(childCenter_[i], halfSize, level_ + 1, this); }

            child_[i]->addTriangle(tri);
          }
        }
      }
      list_.clear();
    }
  }
  else
  {
    glm::vec3 *vertices = triangle.getVertices();

    // triangle can be stored in more than one child node if the size is too large
    for (unsigned int i = 0; i < 8; i++)
    {
      // test if triangle belongs to that sub cube
      if (triangleInsideAABB(vertices, childCenter_[i], halfSize))
      {
        if (child_[i] == NULL)
        { child_[i] = new OctreeNode(childCenter_[i], halfSize, level_ + 1, this); }

        child_[i]->addTriangle(triangle);
      }
    }
  }
}

bool acgm::OctreeNode::triangleInsideAABB(glm::vec3 *vertices, glm::vec3 center, glm::vec3 size)
{
  glm::vec3 t0 = vertices[0];
  if (pointInsideAABB(t0, center, size))
  { return true; }

  glm::vec3 t1 = vertices[1];
  if (pointInsideAABB(t1, center, size))
  { return true; }

  glm::vec3 t2 = vertices[2];
  if (pointInsideAABB(t2, center, size))
  { return true; }

  glm::vec3 v[3];
  glm::vec3 e[3];
  glm::vec3 f[3];

  // (v) move triangle so that AABB's center is at zero point
  v[0] = t0 - center;
  v[1] = t1 - center;
  v[2] = t2 - center;

  // (e) define AABB's normals
  e[0] = glm::vec3(1.f, 0.f, 0.f);
  e[1] = glm::vec3(0.f, 1.f, 0.f);
  e[2] = glm::vec3(0.f, 0.f, 1.f);

  // (f) get triangle's 3 edges
  f[0] = v[1] - v[0];
  f[1] = v[2] - v[1];
  f[2] = v[0] - v[2];

  // use the 3 edges (f), form rays and make ray/AABB intersection test
  // at least 1 edge has to hit AABB

  if (rayAABBIntersection(Ray(t0, f[0]), center, size))
  { return true; }

  if (rayAABBIntersection(Ray(t1, f[1]), center, size))
  { return true; }

  if (rayAABBIntersection(Ray(t2, f[2]), center, size))
  { return true; }


  // calculate cross product of AABB normal and triangle edge
  // get min and max values
  // calculate "radius" from AABB size
  for (unsigned int i = 0; i < 3; i++)
  {
    for (unsigned int j = 0; j < 3; j++)
    {
      glm::vec3 a = glm::cross(e[i], f[j]);

      float p0 = glm::dot(a, v[0]);
      float p1 = glm::dot(a, v[1]);
      float p2 = glm::dot(a, v[2]);

      float min = glm::min(glm::min(p0, p1), p2);
      float max = glm::max(glm::max(p0, p1), p2);

      float x = a.x < 0.0 ? -a.x : a.x;
      float y = a.y < 0.0 ? -a.y : a.y;
      float z = a.z < 0.0 ? -a.z : a.z;

      a = glm::vec3(x, y, z);

      float rad = glm::dot(size, a);
      if (min > rad || max < -rad)
      { return false; }
    }
  }

  return true;
}

bool acgm::OctreeNode::pointInsideAABB(glm::vec3 point, glm::vec3 center, glm::vec3 size)
{
  glm::vec3 min = center - size;
  glm::vec3 max = center + size;

  return point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y && point.z >= min.z && point.z <= max.z;
}

bool acgm::OctreeNode::rayAABBIntersection(Ray ray, glm::vec3 center, glm::vec3 size)
{
  glm::vec3 start = ray.getOrigin();

  // if starting point is inside axis aligned bounding box, the ray goes through it for sure
  if (pointInsideAABB(start, center, size))
  { return true; }

  glm::vec3 min = center - size;
  glm::vec3 max = center + size;

  float t[6];
  glm::vec3 dir = ray.getDirection();

  // min is used for left, bottom and near plane
  t[0] = (min.x - start.x) / dir.x;
  t[1] = (min.y - start.y) / dir.y;
  t[2] = (min.z - start.z) / dir.z;

  // max is used for right, top and far plane
  t[3] = (max.x - start.x) / dir.x;
  t[4] = (max.y - start.y) / dir.y;
  t[5] = (max.z - start.z) / dir.z;

  /* we have 6 different distance values, and therefore 6 different points
     along the ray. we need to query from x, y and z just 2 values, 1 of them
     is equal to to "min"/"max" (i.e. ray(tl) has min.x, y and z in query)
  */

  glm::vec3 temp;

  for (unsigned int i = 0; i < 6; i++)
  {
    if (t[i] < 0.f)
    { continue; }

    temp = start + t[i] * dir;

    if ((i % 3) == 0)
    {
      // it's left/right, ask for y and z
      if (temp.y >= min.y && temp.y <= max.y &&
        temp.z >= min.z && temp.z <= max.z)
      {
        return true;
      }
    }

    if ((i % 3) == 1)
    {
      // it's bottom/top, ask for x and z
      if (temp.x >= min.x && temp.x <= max.x &&
        temp.z >= min.z && temp.z <= max.z)
      {
        return true;
      }
    }

    if ((i % 3) == 2)
    {
      // it's near/far, ask for x and y
      if (temp.x >= min.x && temp.x <= max.x &&
        temp.y >= min.y && temp.y <= max.y)
      {
        return true;
      }
    }
  }

  // we never hit bounding box
  return false;
}

acgm::Hit acgm::OctreeNode::Intersect(Ray ray)
{
  float min = FLT_MAX;
  Hit hit;
  hit.t = std::nullopt;

  if (rayAABBIntersection(ray, center_, size_))
  {
    if (!limitReached_ || level_ == LEVEL_MAX)
    {
      for (Triangle triangle : list_)
      {
        Hit h = triangle.Intersect(ray);
        if (h.t && h.t.value() < min)
        {
          min = h.t.value();
          hit = h;
        }
      }
    }
    else
    {
      glm::vec3 halfSize = size_ / 2.f;
      for (unsigned int i = 0; i < 8; i++)
      {
        if (child_[i] && rayAABBIntersection(ray, childCenter_[i], halfSize))
        {
          Hit h = child_[i]->Intersect(ray);
          if (h.t && h.t.value() < min)
          {
            min = h.t.value();
            hit = h;
          }
        }
      }
    }
  }
  return hit;
}
