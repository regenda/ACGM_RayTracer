#pragma once

#include <ACGM_RayTracer_lib/Shader.h>

namespace acgm
{
  //! Shader composed of two other shaders can create checkerboard pattern
  class CheckerShader : public Shader
  {
  public:
    CheckerShader(float size, std::shared_ptr<Shader> shader1, std::shared_ptr<Shader> shader2);
    virtual cogs::Color3f CalculateColor(const ShaderInput &input) const override;

    void SetShaders(std::shared_ptr<Shader> shader1, std::shared_ptr<Shader> shader2);

  private:
    float size_;

    std::shared_ptr<Shader> shader_1_;
    std::shared_ptr<Shader> shader_2_;
  };
}