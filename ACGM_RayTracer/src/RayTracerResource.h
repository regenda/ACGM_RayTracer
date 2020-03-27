#pragma once

#include <HIRO/Resource.h>
#include <HIRO_DRAW/renderers/RasterRenderer.h>

#include <ACGM_RayTracer_lib/Scene.h>



//! Data storage and application interface for the RayTracer HIRO module.
class RayTracerResource
  : public hiro::Resource
{
public:
  //! Construct with a name to be displayed in GUI.
  explicit RayTracerResource(const std::string &name);

  hiro::PGadget CreateGadget() override;
  hiro::draw::PRasterRenderer GetRenderer() const;

private:
  //! Raster image which can have individual pixels set.
  hiro::draw::PRasterRenderer raster_renderer_;
};
