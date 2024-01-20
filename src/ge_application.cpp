#include "core/ge_application.hpp"

#include "core/ge_window.hpp"
#include "events/ge_event_type.hpp"
#include "renderer/ge_renderer.hpp"

#include <GLFW/glfw3.h>
#include <renderer/ge_camera.hpp>
#include <renderer/ge_shaders_library.hpp>
#include <renderer/ge_texture_2d.hpp>
#include <utils/ge_ipubsub.hpp>

class CameraChangePub : public IPublisher<Mat4>
{
};

static float pitch = 180;
static float yaw = 0;
static Vec3 cam_pos{ 0, 0, 5 };

struct Application::Impl
{
  Scope<Window> window;
  bool running = true;
  bool minimized = false;
  Camera camera{ cam_pos, pitch, yaw, 0 };
  Ref<Texture2D> tex;
  ResizeChangePub resize_change_pub{};
  CameraChangePub camera_change_pub{};
  bool is_mouse_pressed = false;
  Vec2 mouse_press_pos{ 0, 0 };

  static Application* instance;

  void Finish() { running = false; }

  void OnEvent(Event& event)
  {
    Event::Dispatch(EvType::WINDOW_CLOSE,
                    event,
                    [this](auto&&)
                    {
                      Finish();
                      return true;
                    });

    Event::Dispatch(EvType::WINDOW_RESIZE,
                    event,
                    [this](const EvData& ev)
                    {
                      const auto& [_, w, h] = *std::get_if<WindowResizeData>(&ev);
                      Renderer::SetViewport(0, 0, w, h);
                      resize_change_pub.Publish({ (float)w, (float)h });
                      return true;
                    });

    Event::Dispatch(EvType::KEY_PRESS,
                    event,
                    [&](const EvData& ev)
                    {
                      const auto& [_, k] = *std::get_if<KeyPressData>(&ev);
                      const float step = 0.25f;
                      if (k == GLFW_KEY_W)
                        cam_pos.z -= step;
                      if (k == GLFW_KEY_S)
                        cam_pos.z += step;
                      if (k == GLFW_KEY_A)
                        cam_pos.x -= step;
                      if (k == GLFW_KEY_D)
                        cam_pos.x += step;

                      camera = Camera{ cam_pos, pitch, yaw, 0 };
                      camera_change_pub.Publish(camera.GetViewProjection());
                      return true;
                    });

    Event::Dispatch(EvType::MOUSE_BUTTON_PRESSED,
                    event,
                    [&](const EvData& ev)
                    {
                      const auto& [_, bt] = *std::get_if<MouseButtonPressData>(&ev);
                      if (bt == GLFW_MOUSE_BUTTON_1)
                      {
                        is_mouse_pressed = true;
                        mouse_press_pos = window->GetCursorPos();
                      }
                      return true;
                    });

    Event::Dispatch(EvType::MOUSE_BUTTON_RELEASE,
                    event,
                    [&](const EvData& ev)
                    {
                      const auto& [_, bt] = *std::get_if<MouseButtonReleaseData>(&ev);
                      if (bt == GLFW_MOUSE_BUTTON_1)
                      {
                        is_mouse_pressed = false;

                        const float diff_x = mouse_press_pos.x - window->GetCursorPos().x;
                        const float diff_y = mouse_press_pos.y - window->GetCursorPos().y;

                        const float diff_x_limited =
                          std::clamp(diff_x,
                                     (float)(window->GetWidth() / -2.0f),
                                     (float)(window->GetWidth() / 2.0f)) /
                          400.0f;
                        const float diff_y_limited =
                          std::clamp(diff_y,
                                     (float)(window->GetHeight() / -2.0f),
                                     (float)(window->GetHeight() / 2.0f)) /
                          400.0f;

                        const float diff_pitch = +diff_y_limited * 50;
                        const float diff_yaw = -diff_x_limited * 50;

                        pitch += diff_pitch;
                        yaw += diff_yaw;

                        printf("%1.3f, %1.3f\n", pitch, yaw);
                      }
                      return true;
                    });

    Event::Dispatch(EvType::MOUSE_MOVE,
                    event,
                    [&](const EvData& ev)
                    {
                      if (!is_mouse_pressed)
                        return false;
                      const auto& [_, x, y] = *std::get_if<MouseMoveData>(&ev);

                      const float diff_x = mouse_press_pos.x - x;
                      const float diff_y = mouse_press_pos.y - y;

                      const float diff_x_limited = std::clamp(diff_x,
                                                              (float)(window->GetWidth() / -2.0f),
                                                              (float)(window->GetWidth() / 2.0f)) /
                                                   400.0f;
                      const float diff_y_limited = std::clamp(diff_y,
                                                              (float)(window->GetHeight() / -2.0f),
                                                              (float)(window->GetHeight() / 2.0f)) /
                                                   400.0f;

                      const float diff_pitch = +diff_y_limited * 50;
                      const float diff_yaw = -diff_x_limited * 50;

                      printf("%1.3f, %1.3f\n", diff_pitch, diff_yaw);

                      camera = Camera{ cam_pos, pitch + diff_pitch, yaw + diff_yaw, 0 };
                      camera_change_pub.Publish(camera.GetViewProjection());

                      return true;
                    });
  }
};

Application* Application::Impl::instance = nullptr;

Application::Application(std::string&& title, u32 width, u32 height)
{
  m_pimpl = MakeScope<Impl>();
  if (m_pimpl->instance != nullptr)
    throw std::exception("App already instantiated!");

  m_pimpl->instance = this;

  m_pimpl->window = MakeScope<Window>(WindowProps{ title, width, height },
                                      [this](Event& e) { m_pimpl->OnEvent(e); });

  Renderer::Init();
  Renderer::SetViewport(0, 0, width, height);

  ShadersLibrary::Get().SubToCameraPub(m_pimpl->camera_change_pub);
  ShadersLibrary::Get().SubToResizePub(m_pimpl->resize_change_pub);

  m_pimpl->tex = MakeScope<Texture2D>("assets/dice.jpg");
  m_pimpl->tex->Bind(0);

  //  {
  ////    Vec3 eye{ 0.0f, 4.0f, 4.0f };
  ////    //    Vec3 target{ 0, 0, 0 };
  ////    //    Vec3 dir = (target - eye).Normalize();
  //    m_pimpl->camera = Camera{
  //      eye,
  //      90.0f,
  //      0.0,
  //      0,
  //    };
  //  }

  m_pimpl->camera_change_pub.Publish(m_pimpl->camera.GetViewProjection());

  m_pimpl->resize_change_pub.Publish(
    Vec2{ (float)m_pimpl->window->GetWidth(), (float)m_pimpl->window->GetHeight() });

  //  {
  //    auto sh = std::static_pointer_cast<PosAndColorShader>(
  //      ShadersLibrary::Get().GetShader(Shaders::POSITION_AND_COLOR));
  //    sh->Activate();
  //    sh->UploadWidth((float)m_pimpl->window->GetWidth());
  //    sh->UploadHeight((float)m_pimpl->window->GetHeight());
  //  }
}

Application::~Application() = default;

void Application::Run(const std::function<void(Window&)>& onLoop)
{
  while (m_pimpl->running)
  {
    if (!m_pimpl->minimized)
    {
      onLoop(std::ref(*m_pimpl->window));

      m_pimpl->window->OnUpdate();
    }
  }
}
