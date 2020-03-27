#include <ACGM_RayTracer_lib/CheckerShader.h>

acgm::CheckerShader::CheckerShader(float size, std::shared_ptr<Shader> shader1, std::shared_ptr<Shader> shader2)
  : size_(size), shader_1_(shader1), shader_2_(shader2)
{
}

void acgm::CheckerShader::SetShaders(std::shared_ptr<Shader> shader1, std::shared_ptr<Shader> shader2)
{
  shader_1_ = shader1;
  shader_2_ = shader2;
}

cogs::Color3f acgm::CheckerShader::CalculateColor(const ShaderInput &input) const
{
  float bias = 0.001f;

  if ((int)(floor(input.point.x / size_ + bias) + floor(input.point.y / size_ + bias) + floor(input.point.z / size_ + bias)) % 2 == 0)
  {
    return shader_1_->CalculateColor(input);
  }
  else
  {
    return shader_2_->CalculateColor(input);
  }
}
