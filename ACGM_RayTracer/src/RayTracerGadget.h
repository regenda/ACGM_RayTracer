#pragma once

#include <HIRO/Gadget.h>

#include <HIRO_DRAW/renderers/RasterRenderer.h>

#include <ACGM_RayTracer_lib/Scene.h>
#include <ACGM_RayTracer_lib/SceneImporter.h>



//! Visual part of the RayTracer HIRO module.
class RayTracerGadget
  : public hiro::Gadget
{
public:
  //! Construct with a HIRO resource.
  explicit RayTracerGadget(const hiro::Resource *res);

  void Initialize() override;

  void GenerateGui(hiro::GuiGenerator &gui) override;

  void Raytrace(const hiro::gui::Button *btn) const;

  void ChangeModel(const hiro::gui::Droplist *drp);

private:
  //! Structure specifying how the raster should be rendered.
  hiro::draw::PRasterStyle style_;

  //acgm::Scene scene;
  acgm::SceneImporter *sceneImporter;

  hiro::gui::Droplist *model_selector_;
  hiro::gui::Button *render_button_;
  hiro::gui::Checkbox *octree_check_;
  hiro::gui::Checkbox *smooth_check_;
  hiro::gui::NumericInt *reflection_depth_, *refraction_depth_;
};
