#include "pch.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <ACGM_RayTracer_lib/Scene.h>

// calculate uv coordinates from view direction
TEST(UVtests, UVcoordinatesTest)
{
  glm::vec3 enviro_up(0.f, 1.f, 0.f), enviro_seam(-0.707107f, 0.f, -0.707107f), view(0.894427f, 0.447214f, 0.f);

  std::shared_ptr<acgm::Scene> scene = std::make_shared<acgm::Scene>(nullptr, nullptr, std::vector<std::shared_ptr<acgm::Model>>(), "", enviro_up, enviro_seam, 1.0f, epsilon);

  glm::vec2 uvs = scene->CalculateUV(view);

  EXPECT_TRUE(glm::all(glm::epsilonEqual(
        glm::vec2(0.875f, 0.352f),
        uvs,
        epsilon)));
}
