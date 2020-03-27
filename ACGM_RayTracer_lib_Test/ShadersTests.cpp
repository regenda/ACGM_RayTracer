#include "pch.h"

#include <ACGM_RayTracer_lib/Shader.h>
#include <ACGM_RayTracer_lib/ShaderInput.h>
#include <ACGM_RayTracer_lib/PhongShader.h>
#include <ACGM_RayTracer_lib/CheckerShader.h>


// Blinn-Phong test outside the shadow
TEST(ShadersTests, PhongShaderCalculateColorTest1)
{
  std::shared_ptr<acgm::Shader> phong = std::make_shared<acgm::PhongShader>(0.5f, 0.5f, 50.f, 0.2f);
  phong->SetColor(cogs::color::RED);

  acgm::ShaderInput shaderInput;
  shaderInput.point = glm::vec3(0.f, 1.f, -1.f);
  shaderInput.direction_to_eye = glm::vec3(0.f, -1.f, 2.f) / sqrt(5.f);
  shaderInput.direction_to_light = glm::vec3(0.2f, 1.f, 0.f) / sqrt(1.04f);
  shaderInput.light_intensity = 0.7f;
  shaderInput.is_point_in_shadow = false;
  shaderInput.normal = glm::vec3(0.1f, 0.5f, 1.f);

  cogs::Color3f color = phong->CalculateColor(shaderInput);

  EXPECT_TRUE(
    glm::epsilonEqual<float>(0.9f, color.r, epsilon) &&
    glm::epsilonEqual<float>(0.7f, color.g, epsilon) &&
    glm::epsilonEqual<float>(0.7f, color.b, epsilon)
  );
}

// Blinn-Phong test in shadow
TEST(ShadersTests, PhongShaderCalculateColorTest2)
{
  std::shared_ptr<acgm::Shader> phong = std::make_shared<acgm::PhongShader>(0.5f, 0.5f, 50.f, 0.2f);
  phong->SetColor(cogs::color::RED);

  acgm::ShaderInput shaderInput;
  shaderInput.point = glm::vec3(0.f, 1.f, -1.f);
  shaderInput.direction_to_eye = glm::vec3(0.f, -1.f, 2.f) / sqrt(5.f);
  shaderInput.direction_to_light = glm::vec3(0.2f, 1.f, 0.f) / sqrt(1.04f);
  shaderInput.light_intensity = 0.7f;
  shaderInput.is_point_in_shadow = true;
  shaderInput.normal = glm::vec3(0.1f, 0.5f, 1.f);

  cogs::Color3f color = phong->CalculateColor(shaderInput);

  EXPECT_TRUE(
    glm::epsilonEqual<float>(0.2f, color.r, epsilon) &&
    glm::epsilonEqual<float>(0.f, color.g, epsilon) &&
    glm::epsilonEqual<float>(0.f, color.b, epsilon)
  );
}

// Checker shader test - first phong outside the shadow
TEST(ShadersTests, CheckerShaderCalculateColorTest1)
{
  std::shared_ptr<acgm::Shader> phong1 = std::make_shared<acgm::PhongShader>(0.5f, 0.5f, 50.f, 0.2f);
  phong1->SetColor(cogs::color::RED);
  std::shared_ptr<acgm::Shader> phong2 = std::make_shared<acgm::PhongShader>(0.25f, 0.6f, 100.f, 0.15f);
  phong2->SetColor(cogs::color::BLUE);

  std::shared_ptr<acgm::Shader> checker = std::make_shared<acgm::CheckerShader>(0.8, phong1, phong2);

  acgm::ShaderInput shaderInput;
  shaderInput.point = glm::vec3(0.85f, 1.f, -1.f);
  shaderInput.direction_to_eye = glm::vec3(-0.85f, -1.f, 2.f) / sqrt(5.f);
  shaderInput.direction_to_light = glm::vec3(-0.65f, 1.f, 0.f) / sqrt(1.04f);
  shaderInput.light_intensity = 0.6f;
  shaderInput.is_point_in_shadow = false;
  shaderInput.normal = glm::vec3(-0.4f, 0.5f, 1.f);

  cogs::Color3f color = checker->CalculateColor(shaderInput);

  EXPECT_TRUE(
    glm::epsilonEqual<float>(0.8f, color.r, epsilon) &&
    glm::epsilonEqual<float>(0.6f, color.g, epsilon) &&
    glm::epsilonEqual<float>(0.6f, color.b, epsilon)
  );
}

// Checker shader test - second phong in shadow
TEST(ShadersTests, CheckerShaderCalculateColorTest2)
{
  std::shared_ptr<acgm::Shader> phong1 = std::make_shared<acgm::PhongShader>(0.5f, 0.5f, 50.f, 0.2f);
  phong1->SetColor(cogs::color::RED);
  std::shared_ptr<acgm::Shader> phong2 = std::make_shared<acgm::PhongShader>(0.25f, 0.6f, 100.f, 0.15f);
  phong2->SetColor(cogs::color::BLUE);

  std::shared_ptr<acgm::Shader> checker = std::make_shared<acgm::CheckerShader>(0.8, phong1, phong2);

  acgm::ShaderInput shaderInput;
  shaderInput.point = glm::vec3(0.f, 1.f, -1.f);
  shaderInput.direction_to_eye = glm::vec3(0.f, -1.f, 2.f) / sqrt(5.f);
  shaderInput.direction_to_light = glm::vec3(0.2f, 1.f, 0.f) / sqrt(1.04f);
  shaderInput.light_intensity = 0.7f;
  shaderInput.is_point_in_shadow = true;
  shaderInput.normal = glm::vec3(0.1f, 0.5f, 1.f);

  cogs::Color3f color = checker->CalculateColor(shaderInput);

  EXPECT_TRUE(
    glm::epsilonEqual<float>(0.f, color.r, epsilon) &&
    glm::epsilonEqual<float>(0.f, color.g, epsilon) &&
    glm::epsilonEqual<float>(0.15f, color.b, epsilon)
  );
}
