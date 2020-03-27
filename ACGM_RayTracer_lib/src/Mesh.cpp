#include <ACGM_RayTracer_lib/Mesh.h>
#include <ACGM_RayTracer_lib/Triangle.h>

acgm::Mesh::Mesh(
  glm::vec3 position,
  std::string model,
  const cogs::Color3f &color)
  : Model(color)
{
  mesh_.Import(model);
  float rotX = -90.f;
  float rotY = 180.f;
  float rotZ = 0.f;
  transform_.SetScaling(glm::vec3(1.f, 1.f, 1.f));
  transform_.SetRotation(glm::quat(glm::vec3(glm::radians(rotX), glm::radians(rotY), glm::radians(rotZ))));
  transform_.SetPosition(position);
  mesh_.points->Transform(transform_.GetMat4());
  InitializeOctree();
}

acgm::Mesh::Mesh(std::string model, glm::mat4 transform, bool octree)
  : transform_(transform)
{
  mesh_.Import(model);
  mesh_.points->Transform(transform_);
  if(octree)
  {
    InitializeOctree();
  }
  else
  {
    root_ = NULL;
  }
}

cogs::Mesh acgm::Mesh::getMesh()
{
  return mesh_;
}

cogs::Transform acgm::Mesh::getTransform()
{
  return transform_;
}

acgm::Hit acgm::Mesh::Intersect(const acgm::Ray &ray) const
{
  // if octree is initialized
  if(root_)
  { return root_->Intersect(ray); }

  std::vector<glm::uvec3> faceIndices = mesh_.faces->GetFaces();
  glm::vec3 *vertices = mesh_.points->GetPositions();

  float min = FLT_MAX;
  Hit hit;
  hit.t = std::nullopt;

  // traverse all triangles and find closest intersection with ray
  for (int i = 0; i < faceIndices.size(); i++)
  {
    glm::vec3 face[3];
    face[0] = vertices[faceIndices[i].x];
    face[1] = vertices[faceIndices[i].y];
    face[2] = vertices[faceIndices[i].z];

    Triangle triangle = Triangle(face, Color());

    Hit h = triangle.Intersect(ray);
    if (h.t && h.t.value() < min)
    {
      min = h.t.value();
      hit = h;
    }
  }
  return hit;
}

void acgm::Mesh::InitializeOctree()
{
  glm::vec3 *vertices = mesh_.points->GetPositions();
  float inf = FLT_MAX;

  float xmin = inf, ymin = inf, zmin = inf;
  float xmax = -inf, ymax = -inf, zmax = -inf;

  // find boundary points
  for (unsigned int i = 0; i < mesh_.points->GetSize(); i++)
  {
    glm::vec3 p = vertices[i];

    if (p.x < xmin)
    { xmin = p.x; }

    if (p.y < ymin)
    { ymin = p.y; }

    if (p.z < zmin)
    { zmin = p.z; }

    if (p.x > xmax)
    { xmax = p.x; }

    if (p.y > ymax)
    { ymax = p.y; }

    if (p.z > zmax)
    { zmax = p.z; }
  }

  glm::vec3 center((xmax + xmin) / 2.f, (ymax + ymin) / 2.f, (zmax + zmin) / 2.f);
  glm::vec3 size((xmax - xmin) / 2.f + 0.001f, (ymax - ymin) / 2.f + 0.001f, (zmax - zmin) / 2.f + 0.001f);

  root_ = new OctreeNode(center, size, 0);
  BuildOctree();
}

void acgm::Mesh::BuildOctree()
{
  std::vector<glm::uvec3> faceIndices = mesh_.faces->GetFaces();
  glm::vec3 *vertices = mesh_.points->GetPositions();

  for (int i = 0; i < faceIndices.size(); i++)
  {
    glm::vec3 face[3];
    face[0] = vertices[faceIndices[i].x];
    face[1] = vertices[faceIndices[i].y];
    face[2] = vertices[faceIndices[i].z];

    // add triangle to octree
    Triangle triangle = Triangle(face, Color());
    root_->addTriangle(triangle);
  }
}
