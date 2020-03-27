#include <ACGM_RayTracer_lib/PhongShader.h>
#include <glm/glm.hpp>

acgm::PhongShader::PhongShader(float kd, float ks, float ns, float ambient)
  : k_d_(kd), k_s_(ks), n_s_(ns), ambient_(ambient)
{
}

cogs::Color3f acgm::PhongShader::CalculateColor(const ShaderInput &input) const
{
  // Phong Diffuse
  cogs::Color3f diffuse = k_d_ * glm::dot(input.direction_to_light, input.normal) * Color();

  // half vector
  glm::vec3 h = glm::normalize(input.direction_to_light + input.direction_to_eye);
  // Blinn-Phong Specular
  cogs::Color3f specular = k_s_ * glm::pow(glm::dot(h, input.normal), n_s_) * cogs::color::WHITE;

  return ambient_ * Color() + !input.is_point_in_shadow * (diffuse + specular) * input.light_intensity;
}
