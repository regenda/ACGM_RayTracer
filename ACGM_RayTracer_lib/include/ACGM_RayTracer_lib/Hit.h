#pragma once

#include <optional>
#include <glm/common.hpp>

namespace acgm
{
  //! return from Intersect method
  struct Hit
  {
    std::optional<float> t;
    glm::vec3 normal;
  };
}