#include <ACGM_RayTracer_lib/Shader.h>

const cogs::Color3f &acgm::Shader::Color() const
{
  return color_;
}

void acgm::Shader::SetColor(cogs::Color3f color)
{
  color_ = color;
}
