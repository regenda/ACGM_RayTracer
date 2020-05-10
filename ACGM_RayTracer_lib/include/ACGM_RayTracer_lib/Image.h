#pragma once

#include <COGS/Color.h>

namespace acgm
{
  //! Image representation
  class Image
  {
  public:
    explicit Image(std::string file_path);
    ~Image();
    // calculate color from uv coordinates
    cogs::Color3f GetColorAt(const glm::vec2 &uvs) const;
  private:
    uint8_t *data_;
    int width_;
    int height_;
    int bpp_;
  };
}
