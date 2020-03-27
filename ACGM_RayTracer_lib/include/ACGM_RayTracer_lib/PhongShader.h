#pragma once

#include <ACGM_RayTracer_lib/Shader.h>

namespace acgm
{
  //! Shader based on Blinn-Phong reflection model
  class PhongShader : public Shader
  {
  public:
    PhongShader(float kd, float ks, float ns, float ambient);
    virtual cogs::Color3f CalculateColor(const ShaderInput &input) const override;

  private:
    // constants
    float k_d_; // diffuse
    float k_s_; // specular
    float n_s_; // shininess
    float ambient_; // ambient
  };
}