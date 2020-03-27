#pragma once

#include <COGS/Color.h>
#include <ACGM_RayTracer_lib/ShaderInput.h>

namespace acgm
{
  //! Shader  - abstract base class for shaders
  class Shader
  {
  public:
    virtual ~Shader() = default;

    virtual cogs::Color3f CalculateColor(const ShaderInput &input) const = 0;

    void SetColor(cogs::Color3f color);

    //! Get shader color
    const cogs::Color3f &Color() const;

  private:
    cogs::Color3f color_;
  };
}
