#pragma once

#include <vector>
#include <HIRO_DRAW/renderers/RasterRenderer.h>

#include <ACGM_RayTracer_lib/Camera.h>
#include <ACGM_RayTracer_lib/Model.h>
#include <ACGM_RayTracer_lib/Light.h>



namespace acgm
{
  //! Scene representation
  class Scene
  {
  public:
    Scene();
    Scene(std::shared_ptr<Camera> camera, std::shared_ptr<Light> light, std::vector<std::shared_ptr<Model>> models);
    ~Scene() = default;

    //! RayTrace the scene
    void Raytrace(hiro::draw::RasterRenderer &renderer) const;

    // for prototyping
    void ChangeModel(std::string model);

  private:
    std::shared_ptr<Camera> camera_;
    std::shared_ptr<Light> light_;
    std::vector<std::shared_ptr<Model>> models_;
  };
}
