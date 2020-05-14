#pragma once

#include <vector>
#include <HIRO_DRAW/renderers/RasterRenderer.h>

#include <ACGM_RayTracer_lib/Camera.h>
#include <ACGM_RayTracer_lib/Model.h>
#include <ACGM_RayTracer_lib/Light.h>
#include <ACGM_RayTracer_lib/Image.h>


namespace acgm
{
  //! Scene representation
  class Scene
  {
  public:
    Scene();
    Scene(
      std::shared_ptr<Camera> camera,
      //std::shared_ptr<Light> light,
      std::vector<std::shared_ptr<Light>> lights,
      std::vector<std::shared_ptr<Model>> models,
      std::string enviro_image_file,
      glm::vec3 enviro_up,
      glm::vec3 enviro_seam,
      float index_of_refraction,
      float bias);
    ~Scene() = default;

    //! RayTrace the scene
    void Raytrace(hiro::draw::RasterRenderer &renderer) const;

    // for prototyping
    void ChangeModel(std::string model);

    // compute uv coordinates from view direction for environment mapping
    glm::vec2 CalculateUV(glm::vec3 &view) const;

    // set maximum depth for reflection and refraction rays
    void SetDepths(const int reflection_max, const int refraction_max);

    // compute reflected ray
    Ray Reflect(glm::vec3 &intersectPoint, glm::vec3 &dir, glm::vec3 &normal, float cos1) const;

    // compute refracted ray
    std::optional<Ray> Refract(glm::vec3 &intersectPoint, glm::vec3 &dir, glm::vec3 &normal, float cos1, float eta) const;

  private:
    cogs::Color3f Trace(const acgm::Ray &ray, const int reflection_depth, const int refraction_depth, const float product) const;

    std::shared_ptr<Camera> camera_;
    //std::shared_ptr<Light> light_;
    std::vector <std::shared_ptr<Light>> lights_;
    std::vector<std::shared_ptr<Model>> models_;
    std::shared_ptr<Image> image_;
    std::string enviro_image_file_;
    glm::vec3 enviro_up_, enviro_seam_;
    float index_of_refraction_, bias_;
    int reflection_max_, refraction_max_;
  };
}
