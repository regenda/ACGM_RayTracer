#include <ACGM_RayTracer_lib/Image.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <ACGM_RayTracer_lib/stb_image.h>


acgm::Image::Image(std::string file_path)
{
  data_ = stbi_load(file_path.c_str(), &width_, &height_, &bpp_, 3);
  if (!data_)
  {
    std::cout << "Failed to load image " << file_path.c_str() << std::endl;
    std::cout << stbi_failure_reason() << std::endl;
  }
}

acgm::Image::~Image()
{
  stbi_image_free(data_);
}

cogs::Color3f acgm::Image::GetColorAt(const glm::vec2 &uvs) const
{
  if (!data_)
  {
    return cogs::color::BLACK;
  }

  // get nearest pixel [x,y] from uv
  int x = (int)(uvs.x * width_ + 0.5f);
  int y = (int)(uvs.y * height_ + 0.5f);

  // calculate index of pixel in image data_
  int index = 3 * (y * width_ + x);

  // return float color by converting from uint8_t RGB
  return cogs::Color3f(cogs::Color3b(data_[index], data_[index + 1], data_[index + 2]));
}
