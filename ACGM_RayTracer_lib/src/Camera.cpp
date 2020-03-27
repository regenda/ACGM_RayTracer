#include <ACGM_RayTracer_lib/Camera.h>
#include <glm/glm.hpp>
#include <iostream>


acgm::Camera::Camera(
  const glm::vec3 &position,
  const glm::vec3 &target)
  : position_(position)
  , target_(target)
  , look_(glm::normalize(target - position))
{
  glm::vec3 tmp_up = glm::vec3(0.f, 1.f, 0.f);
  if (glm::all(glm::equal(look_, tmp_up)) || glm::all(glm::equal(look_, -tmp_up)))
  {
    right_ = glm::vec3(1.f, 0.f, 0.f);
  }
  else
  {
    right_ = glm::normalize(glm::cross(look_, tmp_up));
  }
  up_ = glm::normalize(glm::cross(right_, look_));
}

acgm::Camera::Camera(
  const glm::vec3 &position,
  const glm::vec3 &up,
  const glm::vec3 &look,
  const float z_near,
  const float z_far,
  const float fov)
  : position_(position)
  , up_(up)
  , look_(look)
  , z_near_(z_near)
  , z_far_(z_far)
  , fov_(fov)
{
  right_ = glm::normalize(glm::cross(look_, up_));
}

const glm::vec3 &acgm::Camera::Position() const
{
  return position_;
}

const glm::vec3 acgm::Camera::getUp() const
{
  return up_;
}

const glm::vec3 acgm::Camera::getLook() const
{
  return look_;
}

const glm::vec3 acgm::Camera::getRight() const
{
  return right_;
}

const float acgm::Camera::getFov() const
{
  return fov_;
}

const int acgm::Camera::getWidth() const
{
  return width_;
}
const int acgm::Camera::getHeight() const
{
  return height_;
}

const float acgm::Camera::getNear() const
{
  return z_near_;
}

const float acgm::Camera::getFar() const
{
  return z_far_;
}

const float acgm::Camera::getLightEnergy() const
{
  return light_energy_;
}
