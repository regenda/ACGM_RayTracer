#include <ACGM_RayTracer_lib/PhongShader.h>
#include <glm/glm.hpp>

acgm::PhongShader::PhongShader(float kd, float ks, float ns, float ambient)
  : k_d_(kd), k_s_(ks), n_s_(ns), ambient_(ambient)
{
}

acgm::PhongShader::PhongShader(float kd, float ks, float ns, float ambient, float glossiness, float transparency, float refractive_index)
  : k_d_(kd), k_s_(ks), n_s_(ns), ambient_(ambient), glossiness_(glossiness), transparency_(transparency), refractive_index_(refractive_index)
{
}

cogs::Color3f acgm::PhongShader::CalculateColor(const ShaderInput &input) const
{
  cogs::Color3f color = ambient_ * Color();

  // Multiple Light Sources
  for (int i = 0; i < input.is_point_in_shadow.size(); i++)
  {
    if (!input.is_point_in_shadow[i])
    {
      // Phong Diffuse
      cogs::Color3f diffuse = k_d_ * glm::dot(input.direction_to_light[i], input.normal) * Color();

      // half vector
      glm::vec3 h = glm::normalize(input.direction_to_light[i] + input.direction_to_eye);
      // Blinn-Phong Specular
      cogs::Color3f specular = k_s_ * glm::pow(glm::dot(h, input.normal), n_s_) * cogs::color::WHITE;

      color += (diffuse + specular) * input.light_intensity[i];
    }
  }

  return (1.0f - glossiness_ - transparency_) * color;
}

std::vector<float> acgm::PhongShader::getTracingProps(glm::vec3 point) const
{
  std::vector<float> props{ glossiness_, transparency_, refractive_index_ };
  return props;
}
