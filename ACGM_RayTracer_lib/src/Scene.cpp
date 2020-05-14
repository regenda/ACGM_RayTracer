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

#include <glm/gtx/vector_angle.hpp>

// minimal ray intensity contribution for adaptive depth control
constexpr float minProduct = 0.02f;

acgm::Scene::Scene(
  std::shared_ptr<Camera> camera,
  //std::shared_ptr<Light> light,
  std::vector<std::shared_ptr<Light>> lights,
  std::vector<std::shared_ptr<Model>> models,
  std::string enviro_image_file,
  glm::vec3 enviro_up,
  glm::vec3 enviro_seam,
  float index_of_refraction,
  float bias)
  : camera_(camera), /*light_(light), */lights_(lights), models_(models)
  , enviro_image_file_(enviro_image_file), enviro_up_(enviro_up), enviro_seam_(enviro_seam)
  , index_of_refraction_(index_of_refraction), bias_(bias)
{
  if(!enviro_image_file_.empty())
  { image_ = std::make_shared<Image>(enviro_image_file_); }

  //lights_.push_back(light);
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
  dolna->setShader(check);
  models_.push_back(dolna);
  //models_.push_back(std::make_shared<Plane>(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, -1.f, 0.f), cogs::color::GRAY50)); //horna
  //models_.push_back(std::make_shared<Plane>(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 0.f, 1.f), cogs::color::FOREST)); //zadna
  //models_.push_back(std::make_shared<Plane>(glm::vec3(3.f, 0.f, 0.f), glm::vec3(-1.f, 0.f, 0.f), cogs::color::RED)); //prava
  //models_.push_back(std::make_shared<Plane>(glm::vec3(-1.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), cogs::color::SKY)); //lava
  std::shared_ptr<Model> bunny = std::make_shared<Mesh>(glm::vec3(0.f, 0.f, 0.f), "../../3d-models/bunny.fbx", cogs::color::CRIMSON);
  std::shared_ptr<PhongShader> phongBunny = std::make_shared<PhongShader>(0.5f, 0.5f, 50.f, 0.2f);
  phongBunny->SetColor(bunny->Color());
  bunny->setShader(phongBunny);
  models_.push_back(bunny);

  //light_ = std::make_shared<DirectionalLight>(glm::vec3(0.f, -1.f, 0.f), 0.8f);
  //light_ = std::make_shared<PointLight>(2.f, 0.5f, glm::vec3(1.f, 2.f, 0.f), 1.f, 0.f);
  //light_ = std::make_shared<SpotLight>(1.f, 1.f, glm::vec3(-5.f, 5.f, 2.f), 0.005f, 0.005f, glm::vec3(3.f, -5.f, -2.f), 45.f, 2.f);

  camera_ = std::make_shared<Camera>(glm::vec3(0.f, 2.f, 3.5f), glm::vec3(0.f, 0.f, 0.f));
}

void acgm::Scene::ChangeModel(std::string model)
{
  models_.back() = std::make_shared<Mesh>(glm::vec3(0.f, 0.f, 0.f), model, models_.back()->Color());
  std::shared_ptr<PhongShader> phongModel = std::make_shared<PhongShader>(0.5f, 0.5f, 50.f, 0.2f);
  phongModel->SetColor(models_.back()->Color());
  models_.back()->setShader(phongModel);
}

void acgm::Scene::SetDepths(const int reflection_max, const int refraction_max)
{
  reflection_max_ = reflection_max;
  refraction_max_ = refraction_max;
}

void acgm::Scene::Raytrace(hiro::draw::RasterRenderer &renderer) const
{
  //renderer.SetResolution({ camera_->getWidth(), camera_->getHeight() });

  unsigned int resX = renderer.GetResolution().x, resY = renderer.GetResolution().y;

  float tan = glm::tan(camera_->getFov() * 0.5f);
  float aspect = (float)resX / (float)resY;

  // bottom left corner
  float startY = -tan;
  float startX = -aspect * tan;

  // delta computations
  float dY = 2.0f * tan / (float)resY;
  float dX = 2.0f * aspect * tan / (float)resX;

  // we start in the middle of the bottom left pixel
  startY += dY / 2;
  startX += dX / 2;

  glm::vec3 pos = camera_->Position(), look = camera_->getLook(), right = camera_->getRight(), up = camera_->getUp();

  // for each pixel calculate color
  #pragma omp parallel for
  for (int y = 0; y < (int)resY; y++)
  {
    float Y = startY + y * dY;
    float X = startX;
    for (unsigned int x = 0; x < resX; x++, X += dX)
    {
      // construct ray from camera through pixel [x, y]
      glm::vec3 dir = glm::normalize(look + X * right + Y * up);
      Ray ray = Ray(pos, dir, bias_);

      // Ray tracing
      cogs::Color3f color = Trace(ray, 0, 0, 1.0f);

      renderer.SetPixel(x, y, color);
    }
  }
}

glm::vec2 acgm::Scene::CalculateUV(glm::vec3 &view) const
{
  float dot_view_up = glm::dot(view, enviro_up_);
  glm::vec3 X = glm::normalize(view - enviro_up_ * dot_view_up);

  float longitude = glm::orientedAngle(X, enviro_seam_, enviro_up_);
  float latitude = std::acos(dot_view_up);

  // convert longitude and latitude to U and V
  float u = 0.5f + longitude / (2 * glm::pi<float>()); // Longitude ranges <-PI, PI>, convert to <0, 1>
  float v = latitude / glm::pi<float>(); // Latitude ranges <0, PI>, convert to <0, 1>

  return glm::vec2(u, v);
}

cogs::Color3f acgm::Scene::Trace(const acgm::Ray &ray, const int reflection_depth, const int refraction_depth, const float product) const
{
  std::shared_ptr<Model> model;
  glm::vec3 intersectPoint, origin = ray.getOrigin(), dir = ray.getDirection();
  glm::vec3 pos = camera_->Position(), look = camera_->getLook();
  float z_distance, near = camera_->getNear(), far = camera_->getFar();
  Hit hit, h;
  hit.t = FLT_MAX;

  // traverse all objects and find closest intersection with ray
  for (std::shared_ptr<Model> const &m : models_)
  {
    h = m->Intersect(ray);
    if (h.t && h.t.value() < hit.t.value())
    {
      if (reflection_depth == 0 && refraction_depth == 0)
      {
        intersectPoint = origin + h.t.value() * dir;
        z_distance = glm::dot((intersectPoint - pos), look);
        if (z_distance >= near && z_distance <= far)
        {
          hit = h;
          model = m;
        }
      }
      else
      {
        hit = h;
        model = m;
      }
    }
  }

  // ray hits nothing, return black or environment image
  if (hit.t.value() == FLT_MAX)
  {
    if (enviro_image_file_.empty()) { return cogs::color::BLACK; }
    else { return image_->GetColorAt(CalculateUV(dir)); }
  }

  intersectPoint = origin + hit.t.value() * dir;

  // create input for shader
  ShaderInput shaderInput;
  shaderInput.point = intersectPoint;
  shaderInput.direction_to_eye = -dir;
  shaderInput.normal = hit.normal;

  for (std::shared_ptr<Light> const &l : lights_)
  {
    shaderInput.direction_to_light.push_back(l->GetDirectionToLight(intersectPoint));
    shaderInput.light_intensity.push_back(l->GetIntensityAt(intersectPoint));
    shaderInput.is_point_in_shadow.push_back(false);

    // create shadow ray from intersection point to ligth and check possible intersection
    // if there is an intersection, point is in a shadow
    Ray shadowRay = Ray(intersectPoint, shaderInput.direction_to_light.back(), bias_);

    float tToLight = l->GetDistanceToLight(intersectPoint);

    for (std::shared_ptr<Model> const &m : models_)
    {
      h = m->Intersect(shadowRay);
      if (h.t && h.t.value() < tToLight)
      {
        shaderInput.is_point_in_shadow.back() = true;
        break;
      }
    }
  }

  cogs::Color3f color = model->getShader()->CalculateColor(shaderInput);

  if (reflection_depth == reflection_max_ && refraction_depth == refraction_max_)
  {
    return color;
  }

  float cos1 = glm::dot(-dir, hit.normal);

  // test if ray goes from inside
  bool inside = false;
  if (-cos1 > 0)
  {
    inside = true;

    // flip normal
    hit.normal = -hit.normal;
    cos1 = -cos1;
  }

  // get glossiness, transparency and refractive_index of model
  std::vector<float> props = model->getShader()->getTracingProps(intersectPoint);
  float glossiness = props[0];
  float transparency = props[1];
  float refractive_index = props[2];

  /*// Fresnel effect using Schlick’s Approximation
    float R_0 = (inside) ?
    (refractive_index - index_of_refraction_) / (refractive_index + index_of_refraction_) :
    (index_of_refraction_ - refractive_index) / (index_of_refraction_ + refractive_index);
    R_0 *= R_0;
    float R_theta = R_0 + (1.f - R_0) * std::pow(1.f - cos1, 5);*/

  float eta = (inside) ? (refractive_index / index_of_refraction_) : (index_of_refraction_ / refractive_index);

  std::optional<Ray> refractionRay = Refract(intersectPoint, dir, hit.normal, cos1, eta);

  // refraction ray contribution
  float productT = product * transparency;

  bool total = false;
  if (!refractionRay)
  {
    // total internal reflection
    total = true;
  }
  else
  {
    // trace refraction ray
    if (refraction_depth < refraction_max_ && productT >= minProduct)
    {
      color += transparency * Trace(refractionRay.value(), reflection_depth, refraction_depth + 1, productT);
      //color += (1.f - R_theta) * transparency * Trace(refractionRay.value(), reflection_depth, refraction_depth + 1, productT);
    }
  }

  if (reflection_depth < reflection_max_)
  {
    // reflection ray contribution
    float productG = product * glossiness;
    if (!total)
    {
      // trace reflection ray if its contribution is big enough
      if(productG >= minProduct)
      {
        Ray reflectionRay = Reflect(intersectPoint, dir, hit.normal, cos1);
        color += glossiness * Trace(reflectionRay, reflection_depth + 1, refraction_depth, productG);
        //color += R_theta * glossiness * Trace(reflectionRay, reflection_depth + 1, refraction_depth, productG);
      }
    }
    else // total internal reflection
    {
      float together = productG + productT;
      // trace reflection ray if its contribution together with transparency contribution is big enough
      if (together >= minProduct)
      {
        Ray reflectionRay = Reflect(intersectPoint, dir, hit.normal, cos1);
        color += (glossiness + transparency) * Trace(reflectionRay, reflection_depth + 1, refraction_depth, together);
        //color += /*R_theta * */(glossiness + transparency) * Trace(reflectionRay, reflection_depth + 1, refraction_depth, together);
      }
    }
  }

  return color;
}

acgm::Ray acgm::Scene::Reflect(glm::vec3 &intersectPoint, glm::vec3 &dir, glm::vec3 &normal, float cos1) const
{
  glm::vec3 reflectDir = 2 * cos1 * normal + dir;

  return Ray(intersectPoint, reflectDir, bias_);
}

std::optional<acgm::Ray> acgm::Scene::Refract(glm::vec3 &intersectPoint, glm::vec3 &dir, glm::vec3 &normal, float cos1, float eta) const
{
  float cos_theta_2_sqr = 1.0f - eta * eta * (1.0f - cos1 * cos1);
  if (cos_theta_2_sqr <= 0)
  {
    // total internal reflection
    return std::nullopt;
  }
  glm::vec3 refractDir = glm::normalize(eta * dir + normal * (eta * cos1 - sqrtf(cos_theta_2_sqr)));

  return Ray(intersectPoint, refractDir, bias_);
}
