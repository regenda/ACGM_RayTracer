#include "pch.h"

#include <ACGM_RayTracer_lib/DirectionalLight.h>
#include <ACGM_RayTracer_lib/PointLight.h>
#include <ACGM_RayTracer_lib/SpotLight.h>


// Directional light - same intensity everywhere
TEST(LightsTests, DirectionalLightIntensityTest)
{
  std::shared_ptr<acgm::Light> light = std::make_shared<acgm::DirectionalLight>(glm::vec3(0.f, -1.f, 0.f), 0.8f);
  EXPECT_TRUE(glm::epsilonEqual<float>(
      0.8f,
      light->GetIntensityAt(glm::vec3(-2.f, 3.f, -1.f)),
      glm::epsilon<float>()));
}

// Point light - intensity decreases with distance
TEST(LightsTests, PointLightIntensityTest)
{
  std::shared_ptr<acgm::Light> light = std::make_shared<acgm::PointLight>(2.f, 0.5f, glm::vec3(1.f, 2.f, 0.f), 0.9f, 0.1f);
  EXPECT_TRUE(glm::epsilonEqual<float>(
      0.165f,
      light->GetIntensityAt(glm::vec3(-2.f, 3.f, 0.f)),
      epsilon));
}

// Spot light - point in cone
TEST(LightsTests, SpotLightIntensityTest1)
{
  std::shared_ptr<acgm::Light> light = std::make_shared<acgm::SpotLight>(1.5f, 1.f, glm::vec3(-5.f, 5.f, 2.f), 0.05f, 0.05f, glm::vec3(3.f, -5.f, -2.f), 45.f, 2.f);
  EXPECT_TRUE(glm::epsilonEqual<float>(
      0.438f,
      light->GetIntensityAt(glm::vec3(-2.f, 3.f, 0.f)),
      epsilon));
}

// Spot light - outside the cone is intenzity zero
TEST(LightsTests, SpotLightIntensityTest2)
{
  std::shared_ptr<acgm::Light> light = std::make_shared<acgm::SpotLight>(1.5f, 1.f, glm::vec3(-5.f, 5.f, 2.f), 0.05f, 0.05f, glm::vec3(3.f, -5.f, -2.f), 25.f, 2.f);

  EXPECT_EQ(0.f, light->GetIntensityAt(glm::vec3(-2.f, 3.f, 0.f)));
}
