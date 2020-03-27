#pragma once

#include <glm/common.hpp>
#include <glm/glm.hpp>

namespace acgm
{
  //! A Camera representation
  class Camera
  {
  public:
    //! Camera constructors
    Camera(
      const glm::vec3 &position,
      const glm::vec3 &target);

    Camera(
      const glm::vec3 &position,
      const glm::vec3 &up,
      const glm::vec3 &look,
      const float near,
      const float far,
      const float fov);

    //! Get camera position
    const glm::vec3 &Position() const;

    const glm::vec3 getUp() const;
    const glm::vec3 getLook() const;
    const glm::vec3 getRight() const;
    const float getFov() const;
    const float getNear() const;
    const float getFar() const;
    const int getWidth() const;
    const int getHeight() const;
    const float getLightEnergy() const;

  private:
    //! Position in the scene
    const glm::vec3 position_;
    //! Target - the point the camera looks at
    const glm::vec3 target_;

    glm::vec3 up_;
    glm::vec3 look_;
    glm::vec3 right_;
    float fov_ = glm::radians(60.f);
    int width_ = 1280;
    int height_ = 720;
    float z_near_ = 1.f;
    float z_far_ = 10.f;
    float light_energy_ = 0.8f; // for depth shading
  };
}
