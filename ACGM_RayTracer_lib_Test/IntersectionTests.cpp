#include "pch.h"

#include <ACGM_RayTracer_lib/Ray.h>
#include <ACGM_RayTracer_lib/Model.h>
#include <ACGM_RayTracer_lib/Plane.h>
#include <ACGM_RayTracer_lib/Triangle.h>
#include <ACGM_RayTracer_lib/Sphere.h>


// Ray plane inteersection - hit
TEST(IntersectionTests, RayPlaneIntersectionTest1)
{
  std::shared_ptr<acgm::Model> plane = std::make_shared<acgm::Plane>(glm::vec3(0.f, -1.f, 1.f), glm::vec3(0.f, 1.f, 0.f));

  acgm::Hit hit = plane->Intersect(acgm::Ray(glm::vec3(0.f, 0.5f, 1.f), glm::normalize(glm::vec3(0.f, -0.5f, -0.5f)), epsilon));

  EXPECT_TRUE(glm::epsilonEqual<float>(
      3.f / sqrt(2.f),
      hit.t.value(),
      glm::epsilon<float>()));

  EXPECT_TRUE(glm::all(glm::equal(
        glm::vec3(0.f, 1.f, 0.f),
        hit.normal)));
}

// Ray plane inteersection - miss
TEST(IntersectionTests, RayPlaneIntersectionTest2)
{
  std::shared_ptr<acgm::Model> plane = std::make_shared<acgm::Plane>(glm::vec3(0.f, -1.f, 1.f), glm::vec3(0.f, 1.f, 0.f));

  acgm::Hit hit = plane->Intersect(acgm::Ray(glm::vec3(0.f, 0.5f, 1.f), glm::normalize(glm::vec3(0.f, 0.f, -0.5f)), epsilon));

  EXPECT_EQ(hit.t, std::nullopt);
}

// Ray triangle intersection - hit
TEST(IntersectionTests, RayTriangleIntersectionTest1)
{
  glm::vec3 vertices[] = {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, 0.f, 0.f), glm::vec3(0.f, 0.5f, -0.5f)};
  std::shared_ptr<acgm::Model> triangle = std::make_shared<acgm::Triangle>(vertices);

  acgm::Hit hit = triangle->Intersect(acgm::Ray(glm::vec3(0.f, 0.5f, 1.f), glm::normalize(glm::vec3(0.f, -0.5f, -0.5f)), epsilon));

  EXPECT_TRUE(glm::epsilonEqual<float>(
      1.06f,
      hit.t.value(),
      epsilon));

  EXPECT_TRUE(glm::all(glm::epsilonEqual(
        glm::vec3(0.f, 0.75f, 0.75f) / sqrt(1.125f),
        hit.normal,
        glm::epsilon<float>())));
}

// Ray triangle intersection - miss
TEST(IntersectionTests, RayTriangleIntersectionTest2)
{
  glm::vec3 vertices[] = { glm::vec3(-2.5f, -2.5f, 0.5f), glm::vec3(-1.5f, -2.f, 0.f), glm::vec3(-2.f, -1.5f, -0.5f) };
  std::shared_ptr<acgm::Model> triangle = std::make_shared<acgm::Triangle>(vertices);

  acgm::Hit hit = triangle->Intersect(acgm::Ray(glm::vec3(0.f, 0.5f, 1.f), glm::normalize(glm::vec3(0.f, -0.5f, -0.5f)), epsilon));

  EXPECT_EQ(hit.t, std::nullopt);
}

// Ray sphere intersection - hit
TEST(IntersectionTests, RaySphereIntersectionTest1)
{
  std::shared_ptr<acgm::Model> sphere = std::make_shared<acgm::Sphere>(glm::vec3(0.f, 0.f, -1.5f), 1.5f);

  acgm::Hit hit = sphere->Intersect(acgm::Ray(glm::vec3(0.f, 0.5f, 1.f), glm::normalize(glm::vec3(0.f, -0.5f, -0.5f)), epsilon));

  EXPECT_TRUE(glm::epsilonEqual<float>(
      1.622f,
      hit.t.value(),
      epsilon));

  EXPECT_TRUE(glm::all(glm::epsilonEqual(
        glm::vec3(0.f, -0.43f, 0.902f),
        hit.normal,
        epsilon)));
}

// Ray sphere intersection - miss
TEST(IntersectionTests, RaySphereIntersectionTest2)
{
  std::shared_ptr<acgm::Model> sphere = std::make_shared<acgm::Sphere>(glm::vec3(0.f, 2.f, -1.5f), 1.5f);

  acgm::Hit hit = sphere->Intersect(acgm::Ray(glm::vec3(0.f, 0.5f, 1.f), glm::normalize(glm::vec3(0.f, -0.5f, -0.5f)), epsilon));

  EXPECT_EQ(hit.t, std::nullopt);
}
