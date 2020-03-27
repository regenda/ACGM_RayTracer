#pragma once

#include <ACGM_RayTracer_lib/Model.h>
#include <COGS/Mesh.h>
#include <COGS/Transform.h>
#include <ACGM_RayTracer_lib/OctreeNode.h>


namespace acgm
{
  //! Mesh representation
  class Mesh : public Model
  {
  public:
    Mesh(glm::vec3 position, std::string model, const cogs::Color3f &color);
    Mesh(std::string model, glm::mat4 transform, bool octree);
    virtual Hit Intersect(const acgm::Ray &ray) const override;

    cogs::Mesh getMesh();
    cogs::Transform getTransform();
  private:
    cogs::Mesh mesh_;
    cogs::Transform transform_;

    // according to Christian Rumpf
    // https://computergraphicsguide.blogspot.com/2015/09/speed-up-ray-casting-with-octrees.html
    // https://github.com/rumpfc/CGG/tree/master/cgg07_Octrees
    void InitializeOctree();
    void BuildOctree();
    OctreeNode *root_;
  };
}
