#include "EditorLayer.hpp"

static constexpr const auto CLEAR_COLOR = 0x222222FF;

namespace
{
  DISABLE_WARNING_PUSH
  WARN_CONVERSION_OF_GREATER_SIZE
  void* SafeConversion(u32 i)
  {
    return reinterpret_cast<void*>(i);
  }
  DISABLE_WARNING_POP
}

GE::EditorLayer::EditorLayer() : Layer("EditorLayer") {}

void GE::EditorLayer::OnAttach()
{

  m_scene = Scene::Make();

  Ref<EditorCamera> cam = MakeRef<EditorCamera>(45.0f, 1280.0f / 720.0f);
  auto camera_ent = m_scene->CreateEntity("Camera");
  m_scene->AddComponent<EditorCameraComponent>(camera_ent, cam);

  Ref<IShaderProgram> simple_shader = MakeRef<PosAndTex2DShader>();
  Ref<WorldReference> world_reference = MakeRef<WorldReference>(simple_shader, 20);
  auto world_ref_ent = m_scene->CreateEntity("Platform");
  m_scene->AddComponent<PrimitiveComponent>(world_ref_ent, world_reference->GetVAO());
  m_scene->AddComponent<TransformComponent>(world_ref_ent, world_reference->GetModelMatrix());
  m_scene->AddComponent<ColorOnlyComponent>(world_ref_ent, simple_shader);
  //
  //
  //  m_simple_shader = MakeRef<PosAndTex2DShader>();
  //  m_mat_shader = MakeRef<MaterialShader>();
  //  m_light_1 = Cube::Make(Colors::WHITE, m_simple_shader, Texture2D::Make());
  //  m_light_1->SetScale(0.5, 0.5, 0.5);
  //  m_light_2 = Cube::Make(Colors::WHITE, m_simple_shader, Texture2D::Make());
  //  m_light_2->SetScale(0.5, 0.5, 0.5);
  //  m_world_ref = MakeRef<WorldReference>(m_simple_shader, 20);
  //  m_mesh = MakeRef<Mesh>("assets/objs/teapot.obj", m_mat_shader);
  m_fb = Framebuffer::Make(FBSpecs{ 1280, 720, 1, true });
}

void GE::EditorLayer::OnUpdate(GE::TimeStep ts)
{
  const auto& spec = m_fb->GetSpec();
  if (spec.width != i32(m_viewport_width) || spec.height != i32(m_viewport_height))
  {
    m_fb->Resize(static_cast<i32>(m_viewport_width), static_cast<i32>(m_viewport_height));
    m_scene->OnResize(m_viewport_width, m_viewport_height);
  }

  m_fb->Bind();

  //  if (/*m_viewport_focused &&*/ m_viewport_hovered)
  //    m_cam.OnUpdate(ts);

  GE::Renderer::SetClearColor(GE::Color{ CLEAR_COLOR }.ToVec4());
  GE::Renderer::Clear();

  m_scene->OnUpdate(ts);

  //  m_simple_shader->Activate();
  //  m_simple_shader->UpdateViewProjectionMatrix(m_cam.GetViewProjection());
  //  m_mat_shader->Activate();
  //  m_mat_shader->UpdateViewProjectionMatrix(m_cam.GetViewProjection());
  //
  //  m_world_ref->DrawBatch();
  //
  //  // Draw 'light' cube
  //  m_light_1->SetColor(m_light_color_1);
  //  m_light_1->SetTranslate(m_light_pos_1);
  //  m_light_1->Draw();
  //  m_light_2->SetColor(m_light_color_2);
  //  m_light_2->SetTranslate(m_light_pos_2);
  //  m_light_2->Draw();
  //
  //  // Set default ambient light setup
  //  m_mat_shader->Activate();
  //  m_mat_shader->UpdateAmbientColor(m_ambient_color);
  //  m_mat_shader->UpdateAmbientStrength(m_ambient_str);
  //  m_mat_shader->UpdateLightPosition({ m_light_pos_1, m_light_pos_2 });
  //  m_mat_shader->UpdateLightColor({ m_light_color_1, m_light_color_2 });
  //  m_mat_shader->UpdateLightStrength({ m_light_strength, m_light_strength });
  //
  //  m_mesh->Draw();

  m_fb->Unbind();
}

void GE::EditorLayer::OnImGuiUpdate()
{
  static ImGuiDockNodeFlags dock_node_flags = ImGuiDockNodeFlags_None;

  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  ImGuiViewport* vp = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(vp->Pos);
  ImGui::SetNextWindowSize(vp->Size);
  ImGui::SetNextWindowViewport(vp->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
  window_flags |= ImGuiWindowFlags_NoTitleBar              //
                  | ImGuiWindowFlags_NoCollapse            //
                  | ImGuiWindowFlags_NoResize              //
                  | ImGuiWindowFlags_NoMove                //
                  | ImGuiWindowFlags_NoBringToFrontOnFocus //
                  | ImGuiWindowFlags_NoNavFocus;

  if (dock_node_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    window_flags |= ImGuiWindowFlags_NoBackground;

  static auto dockspace_open = true;
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin("Dockspace demo", &dockspace_open, window_flags);
  ImGui::PopStyleVar();

  ImGui::PopStyleVar(2);

  ImGuiIO& io = ImGui::GetIO();
  //  io.IniFilename = nullptr;
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
  {
    ImGuiID dock_id = ImGui::GetID("MyDock");
    ImGui::DockSpace(dock_id, ImVec2{ 0, 0 }, dock_node_flags);
  }

  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("Exit"))
        Ctrl::App::Close();
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

  ImGui::ShowDemoWindow();

  //  ImGui::Begin("Settings");
  //  ImGui::SliderFloat("AmbientStrength", &m_ambient_str, 0, 1);
  //  ImGui::SliderFloat3("LightPos1", &m_light_pos_1.x, -10, 10);
  //  {
  //    GE::Vec3 light_color = m_light_color_1.ToVec3();
  //    ImGui::ColorEdit3("LightColor1", &light_color.x);
  //    m_light_color_1 = GE::Color(light_color);
  //  }
  //  ImGui::SliderFloat3("LightPos2", &m_light_pos_2.x, -10, 10);
  //  {
  //    GE::Vec3 light_color = m_light_color_2.ToVec3();
  //    ImGui::ColorEdit3("LightColor2", &light_color.x);
  //    m_light_color_2 = GE::Color(light_color);
  //  }
  //  ImGui::SliderFloat("LightStrength", &m_light_strength, 0, 10);
  //  ImGui::End();

  ImGui::Begin("Viewport");

  //  m_viewport_focused = ImGui::IsWindowFocused();
  m_viewport_hovered = ImGui::IsWindowHovered();
  Ctrl::App::AllowImGuiEvents(/*!m_viewport_focused ||*/ !m_viewport_hovered);
  auto vp_size = ImGui::GetContentRegionAvail();
  m_viewport_width = u32(vp_size.x);
  m_viewport_height = u32(vp_size.y);
  u32 tex = m_fb->GetColorAttachmentID();
  const auto [w, h] = m_fb->GetSize();
  ImVec2 size{ f32(w), f32(h) };
  ImGui::Image(SafeConversion(tex), size, { 0, 1 }, { 1, 0 });
  ImGui::End();
  ImGui::PopStyleVar();

  ImGui::End();
}

void GE::EditorLayer::OnEvent(GE::Event& e)
{
  m_scene->OnEvent(e);
  //  m_cam.OnEvent(e);
}
