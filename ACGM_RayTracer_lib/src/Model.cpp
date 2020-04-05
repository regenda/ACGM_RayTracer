#include <ACGM_RayTracer_lib/Model.h>



acgm::Model::Model(const cogs::Color3f &color)
  : color_(color)
{
}

acgm::Model::Model()
{
}

const cogs::Color3f &acgm::Model::Color() const
{
  return color_;
}

void acgm::Model::setShader(std::shared_ptr<Shader> shader)
{
  shader_ = shader;
}

std::shared_ptr<acgm::Shader> acgm::Model::getShader()
{
  return shader_;
}
