#pragma once

#include <glm/glm.hpp>
#include <ACGM_RayTracer_lib/Triangle.h>

namespace acgm
{
  // according to Christian Rumpf
  // https://computergraphicsguide.blogspot.com/2015/09/speed-up-ray-casting-with-octrees.html
  // https://github.com/rumpfc/CGG/tree/master/cgg07_Octrees
  class OctreeNode
  {
  public:
    OctreeNode(glm::vec3 &center, glm::vec3 &size, unsigned int level, OctreeNode *parent = NULL);
    ~OctreeNode();

    void addTriangle(Triangle triangle);
    Hit Intersect(Ray ray);

  private:

    static unsigned int LEVEL_MAX; // maximum octree level

    // If LIMIT_MAX is reached, limitReached_ will become true and distributes list_'s
    // content to the 8 child nodes according to their positionings.
    static unsigned int LIMIT_MAX; // maximum triangles in node

    OctreeNode *parent_; // 1 parent octree node
    OctreeNode *child_[8]; // 8 sub octrees

    glm::vec3 size_;
    glm::vec3 center_;

    unsigned int level_;
    glm::vec3 childCenter_[8];

    bool limitReached_;
    std::vector<Triangle> list_;

    bool triangleInsideAABB(glm::vec3 *vertices, glm::vec3 center, glm::vec3 size);
    bool pointInsideAABB(glm::vec3 point, glm::vec3 center, glm::vec3 size);
    bool rayAABBIntersection(Ray ray, glm::vec3 center, glm::vec3 size);
  };
}