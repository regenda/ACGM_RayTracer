#pragma once

#include <COGS/Color.h>
#include <ACGM_RayTracer_lib/Ray.h>
#include <ACGM_RayTracer_lib/Hit.h>
#include <ACGM_RayTracer_lib/Shader.h>


namespace acgm
{
  //! Model  - abstract base class for scene models
  class Model
  {
  public:
    explicit Model(const cogs::Color3f &color);
    Model();
    virtual ~Model() = default;

    virtual Hit Intersect(const acgm::Ray &ray) const = 0;

    //! Get model color
    const cogs::Color3f &Color() const;

    void SetShader(std::shared_ptr<Shader> shader);
    std::shared_ptr<Shader> getShader();

    std::string name;

  protected:
    std::shared_ptr<Shader> shader_;

  private:
    cogs::Color3f color_;
  };
}
