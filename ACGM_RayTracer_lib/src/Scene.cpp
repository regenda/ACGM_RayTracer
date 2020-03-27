#include <ACGM_RayTracer_lib/Scene.h>

#include <ACGM_RayTracer_lib/Plane.h>
#include <ACGM_RayTracer_lib/Mesh.h>
#include <ACGM_RayTracer_lib/DirectionalLight.h>
#include <ACGM_RayTracer_lib/PointLight.h>
#include <ACGM_RayTracer_lib/SpotLight.h>
#include <ACGM_RayTracer_lib/Hit.h>
#include <ACGM_RayTracer_lib/PhongShader.h>
#include <ACGM_RayTracer_lib/ShaderInput.h>
#include <ACGM_RayTracer_lib/CheckerShader.h>

acgm::Scene::Scene(std::shared_ptr<Camera> camera, std::shared_ptr<Light> light, std::vector<std::shared_ptr<Model>> models)
  : camera_(camera), light_(light), models_(models)
{
}

acgm::Scene::Scene()
{
  //models_.reserve(6);
  std::shared_ptr<Model> dolna = std::make_shared<Plane>(glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 1.f, 0.f), cogs::color::GRAY50);
  std::shared_ptr<PhongShader> phong1 = std::make_shared<PhongShader>(0.5f, 0.5f, 50.f, 0.2f);
  phong1->SetColor(cogs::color::WHITE);
  std::shared_ptr<PhongShader> phong2 = std::make_shared<PhongShader>(0.5f, 0.5f, 50.f, 0.2f);
  phong2->SetColor(cogs::color::BLACK);
  std::shared_ptr<CheckerShader> check = std::make_shared<CheckerShader>(1.f, phong1, phong2);
  dolna->SetShader(check);
  models_.push_back(dolna);
  //models_.push_back(std::make_shared<Plane>(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, -1.f, 0.f), cogs::color::GRAY50)); //horna
  //models_.push_back(std::make_shared<Plane>(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 0.f, 1.f), cogs::color::FOREST)); //zadna
  //models_.push_back(std::make_shared<Plane>(glm::vec3(3.f, 0.f, 0.f), glm::vec3(-1.f, 0.f, 0.f), cogs::color::RED)); //prava
  //models_.push_back(std::make_shared<Plane>(glm::vec3(-1.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), cogs::color::SKY)); //lava
  std::shared_ptr<Model> bunny = std::make_shared<Mesh>(glm::vec3(0.f, 0.f, 0.f), "../../3d-models/bunny.fbx", cogs::color::CRIMSON);
  std::shared_ptr<PhongShader> phongBunny = std::make_shared<PhongShader>(0.5f, 0.5f, 50.f, 0.2f);
  phongBunny->SetColor(bunny->Color());
  bunny->SetShader(phongBunny);
  models_.push_back(bunny);

  //light_ = std::make_shared<DirectionalLight>(glm::vec3(0.f, -1.f, 0.f), 0.8f);
  //light_ = std::make_shared<PointLight>(2.f, 0.5f, glm::vec3(1.f, 2.f, 0.f), 1.f, 0.f);
  light_ = std::make_shared<SpotLight>(1.f, 1.f, glm::vec3(-5.f, 5.f, 2.f), 0.005f, 0.005f, glm::vec3(3.f, -5.f, -2.f), 45.f, 2.f);

  camera_ = std::make_shared<Camera>(glm::vec3(0.f, 2.f, 3.5f), glm::vec3(0.f, 0.f, 0.f));
}

void acgm::Scene::ChangeModel(std::string model)
{
  models_.back() = std::make_shared<Mesh>(glm::vec3(0.f, 0.f, 0.f), model, models_.back()->Color());
  std::shared_ptr<PhongShader> phongModel = std::make_shared<PhongShader>(0.5f, 0.5f, 50.f, 0.2f);
  phongModel->SetColor(models_.back()->Color());
  models_.back()->SetShader(phongModel);
}

void acgm::Scene::Raytrace(hiro::draw::RasterRenderer &renderer) const
{
  //renderer.SetResolution({ camera_->getWidth(), camera_->getHeight() });

  float tan = glm::tan(camera_->getFov() * 0.5f);
  float aspect = (float)renderer.GetResolution().x / (float)renderer.GetResolution().y;

  // bottom left corner
  float startY = -tan;
  float startX = -aspect * tan;

  // delta computations
  float dY = 2.0f * tan / (float)renderer.GetResolution().y;
  float dX = 2.0f * aspect * tan / (float)renderer.GetResolution().x;

  // we start in the middle of the bottom left pixel
  startY += dY / 2;
  startX += dX / 2;

  // float z_range = camera_->getFar() - camera_->getNear();

  // for each pixel calculate color
  #pragma omp parallel for
  for (int y = 0; y < (int)renderer.GetResolution().y; y++)
  {
    float Y = startY + y * dY;
    float X = startX;
    for (unsigned int x = 0; x < renderer.GetResolution().x; x++, X += dX)
    {
      // construct ray from camera through pixel [x, y]
      glm::vec3 dir = glm::normalize(camera_->getLook() + X * camera_->getRight() + Y * camera_->getUp());
      Ray ray = Ray(camera_->Position(), dir);

      std::shared_ptr<Model> model;
      Hit hit;
      hit.t = FLT_MAX;

      // traverse all objects and find closest intersection with ray from camera
      for (std::shared_ptr<Model> const &m : models_)
      {
        Hit h = m->Intersect(ray);
        if (h.t && h.t.value() < hit.t.value())
        {
          glm::vec3 intersectPoint = ray.getOrigin() + h.t.value() * ray.getDirection();
          float z_distance = glm::dot((intersectPoint - camera_->Position()), camera_->getLook());
          if (z_distance >= camera_->getNear() && z_distance <= camera_->getFar())
          {
            hit = h;
            model = m;
          }
        }
      }

      // ray hits nothing, set pixel black
      if (hit.t.value() == FLT_MAX)
      {
        renderer.SetPixel(x, y, cogs::color::BLACK);
        continue;
      }

      // point in scene, where ray from camera hits closest object
      glm::vec3 intersectPoint = ray.getOrigin() + hit.t.value() * ray.getDirection();

      // create input for shader
      ShaderInput shaderInput;
      shaderInput.point = intersectPoint;
      shaderInput.direction_to_eye = -dir;
      shaderInput.direction_to_light = light_->GetDirectionToLight(intersectPoint);
      shaderInput.light_intensity = light_->GetIntensityAt(intersectPoint);
      shaderInput.is_point_in_shadow = false;
      shaderInput.normal = hit.normal;

      // create shadow ray from intersection point to ligth and check possible intersection
      // if there is an intersection, point is in a shadow
      Ray shadowRay = Ray(intersectPoint, light_->GetDirectionToLight(intersectPoint));

      float tToLight = glm::distance(intersectPoint, light_->Position());

      for (std::shared_ptr<Model> const &m : models_)
      {
        Hit h = m->Intersect(shadowRay);
        if (h.t && h.t.value() < tToLight)
        {
          shaderInput.is_point_in_shadow = true;
          break;
        }
      }

      // depth shading
      // float t_norm = (hit.t.value() - camera_->getNear()) / z_range;
      // float intensity = camera_->getLightEnergy() / t_norm;
      // cogs::Color3f color = model->getShader()->CalculateColor(shaderInput) * intensity;

      // shader calculates color
      cogs::Color3f color = model->getShader()->CalculateColor(shaderInput);
      renderer.SetPixel(x, y, color);
    }
  }
}
