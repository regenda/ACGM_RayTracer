#include "RayTracerResource.h"
#include "RayTracerGadget.h"



RayTracerGadget::RayTracerGadget(const hiro::Resource *res)
  : hiro::Gadget(res)
{
}

void RayTracerGadget::Initialize()
{
  hiro::Gadget::Initialize();
  style_ = std::make_shared<hiro::draw::RasterStyle>();
  style_->use_nearest_filtering = true;
  AddRenderer(GetResource<RayTracerResource>()->GetRenderer(), style_);

  sceneImporter = new acgm::SceneImporter();
}

void RayTracerGadget::GenerateGui(hiro::GuiGenerator &gui)
{
  /*
    model_selector_ = gui.AddDroplist("Model")
    ->AddItemsIndexed({ "bunny", "sphere",  "monkey" })
    ->Set(0)->Subscribe(std::bind(&RayTracerGadget::ChangeModel, this, std::placeholders::_1));
  */
  /*
    render_button_ = gui.AddButton("Render Scene")->Subscribe([](const hiro::gui::Button * btn)
    {
    //scene.Raytrace(*GetResource<RayTracerResource>()->GetRenderer());
    });
  */
  model_selector_ = gui.AddDroplist("Scene")
    ->AddItemsIndexed({"scene0.txt", "scene1.txt",  "scene2.txt", "scene3.txt", "scene4.txt",  "scene5.txt", "scene6.txt", "scene7.txt", "scene8.txt"})
    ->Set(0);
  reflection_depth_ = gui.AddNumericInt("Max reflection")->SetMinMax(0, 20)->Set(10);
  refraction_depth_ = gui.AddNumericInt("Max transparency")->SetMinMax(0, 20)->Set(10);
  octree_check_ = gui.AddCheckbox("Octree")->Set(false);
  smooth_check_ = gui.AddCheckbox("Smooth")->Set(false);
  render_button_ = gui.AddButton("Render Scene")->Subscribe(std::bind(&RayTracerGadget::Raytrace, this, std::placeholders::_1));
}

void RayTracerGadget::Raytrace(const hiro::gui::Button *btn) const
{
  //scene.Raytrace(*GetResource<RayTracerResource>()->GetRenderer());

  sceneImporter->octree = octree_check_->Get();
  sceneImporter->smooth = smooth_check_->Get();
  sceneImporter->Import(model_selector_->GetText());
  auto renderer = GetResource<RayTracerResource>()->GetRenderer();
  renderer->SetResolution(sceneImporter->GetRenderOptions().resolution);

  auto scene = sceneImporter->GetScene();
  scene->SetDepths(reflection_depth_->Get(), refraction_depth_->Get());
  scene->Raytrace(*renderer);
}

void RayTracerGadget::ChangeModel(const hiro::gui::Droplist *drp)
{
  //std::string model = "../../3d-models/" + drp->GetText() + ".fbx";
  //scene.ChangeModel(model);
}
