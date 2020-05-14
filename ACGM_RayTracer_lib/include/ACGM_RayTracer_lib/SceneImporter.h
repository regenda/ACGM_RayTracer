#pragma once
#include <ACGM_RayTracer_lib/Scene.h>



namespace acgm
{

  struct RenderOptions
  {
    glm::uvec2 resolution;
    uint32_t glossy_bounces;
    uint32_t transparency_bounces;
  };



  class SceneImporter
  {
  public:
    static const int MODELTYPE_PLANE;
    static const int MODELTYPE_SPHERE;
    static const int MODELTYPE_MESH;

    static const int LIGHTTYPE_SUN;
    static const int LIGHTTYPE_POINT;
    static const int LIGHTTYPE_SPOT;

    static const int SHADERTYPE_PHONG;
    static const int SHADERTYPE_CHECKER;

    // whether we want octree speedup for mesh models
    bool octree, smooth;

    SceneImporter() = default;
    ~SceneImporter() = default;
    bool Import(const std::string &filename);
    std::shared_ptr<acgm::Scene> GetScene() const;
    RenderOptions GetRenderOptions() const;

  private:
    std::ifstream stream_;
    std::shared_ptr<acgm::Scene> scene_;
    RenderOptions render_options_;

    std::string GetLine();
    float ReadFloat();
    glm::vec3 ReadVec3();
    glm::mat4 ReadMat4();
    cogs::Color3f ReadColor();
    RenderOptions ReadRenderOptions();
    std::shared_ptr<Camera> ReadCamera();
    std::shared_ptr<Model> ReadModel();
    std::vector<std::shared_ptr<Model>> ReadModels();
    std::shared_ptr<Shader> ReadShader();
    std::shared_ptr<Light> ReadLight();
    std::vector<std::shared_ptr<Light>> ReadLights();
    std::shared_ptr<Scene> ReadScene();
  };

}
