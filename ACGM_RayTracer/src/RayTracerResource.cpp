#include <ACGM_RayTracer_lib/Ray.h>

#include "RayTracerGadget.h"
#include "RayTracerResource.h"




RayTracerResource::RayTracerResource(const std::string &name)
  : hiro::Resource(hiro::ResourceId(name, "RayTracerResource"))
{
  raster_renderer_ = std::make_shared<hiro::draw::RasterRenderer>();

  //raster_renderer_->SetResolution({ 1280, 720 });
}

hiro::PGadget RayTracerResource::CreateGadget()
{
  return std::make_shared<RayTracerGadget>(this);
}

hiro::draw::PRasterRenderer RayTracerResource::GetRenderer() const
{
  return raster_renderer_;
}
