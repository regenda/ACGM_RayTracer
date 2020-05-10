#include "pch.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <ACGM_RayTracer_lib/Scene.h>

// reflect ray
TEST(RayConstructionTests, RayReflectTest)
{
  glm::vec3 intersectPoint(0.f, 0.f, 0.f), dir(0.f, -1.f, 0.f), normal(0.f, 0.707107f, -0.707107f);

  float cos1 = glm::dot(-dir, normal);

  std::shared_ptr<acgm::Scene> scene = std::make_shared<acgm::Scene>();

  acgm::Ray reflectionRay = scene->Reflect(intersectPoint, dir, normal, cos1);

  EXPECT_TRUE(glm::all(glm::epsilonEqual(
        glm::vec3(0.f, 0.f, -1.f),
        reflectionRay.getDirection(),
        epsilon)));
}

// refract ray
TEST(RayConstructionTests, RayRefractTest1)
{
  glm::vec3 intersectPoint(0.f, 0.f, 0.f), dir(0.f, -1.f, 0.f), normal(0.f, 0.707107f, -0.707107f);

  float cos1 = glm::dot(-dir, normal);

  float diamondIndex = 2.418f, glassIndex = 1.5f;
  float eta = glassIndex / diamondIndex;

  std::shared_ptr<acgm::Scene> scene = std::make_shared<acgm::Scene>();

  std::optional<acgm::Ray> refractedRay = scene->Refract(intersectPoint, dir, normal, cos1, eta);

  EXPECT_TRUE(glm::all(glm::epsilonEqual(
        glm::vec3(0.f, -0.946f, 0.325f),
        refractedRay.value().getDirection(),
        epsilon)));
}

// total internal reflection
TEST(RayConstructionTests, RayRefractTest2)
{
  glm::vec3 intersectPoint(0.f, 0.f, 0.f), dir(0.f, -1.f, 0.f), normal(0.f, 0.707107f, -0.707107f);

  float cos1 = glm::dot(-dir, normal);

  float diamondIndex = 2.418f, glassIndex = 1.5f;
  float eta = diamondIndex / glassIndex;

  std::shared_ptr<acgm::Scene> scene = std::make_shared<acgm::Scene>();

  std::optional<acgm::Ray> refractedRay = scene->Refract(intersectPoint, dir, normal, cos1, eta);

  EXPECT_EQ(refractedRay, std::nullopt);
}
