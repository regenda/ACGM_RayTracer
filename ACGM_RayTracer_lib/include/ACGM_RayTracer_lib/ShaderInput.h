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
    std::vector<glm::vec3> direction_to_light;
    std::vector<float> light_intensity;
    std::vector<bool> is_point_in_shadow;
  };
}