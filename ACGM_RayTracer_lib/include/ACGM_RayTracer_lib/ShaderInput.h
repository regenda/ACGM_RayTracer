#pragma once
#include <glm/common.hpp>

namespace acgm
{
  //! everything needed for shader to calculate resulting color
  struct ShaderInput
  {
    glm::vec3 point;
    glm::vec3 normal;
    glm::vec3 direction_to_eye;
    glm::vec3 direction_to_light;
    float light_intensity;
    bool is_point_in_shadow;
  };
}