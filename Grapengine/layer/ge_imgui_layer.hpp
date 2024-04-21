#ifndef GRAPENGINE_GE_IMGUI_LAYER_HPP
#define GRAPENGINE_GE_IMGUI_LAYER_HPP

#include "core/ge_window.hpp"
#include "ge_layer.hpp"

namespace GE
{
  class ImGuiLayer : public Layer
  {
  public:
    static Ref<ImGuiLayer> Make(Ref<Window> window);
    explicit ImGuiLayer(Ref<Window> window);
    ~ImGuiLayer() override;

    void Begin();
    void End();
    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(GE::Event& /*unused*/) override;

    void AllowMouseAndKeyboardEvents(bool allow);

  private:
    Ref<Window> m_window = nullptr;
    bool m_allow_imgui_events = false;
  };
}

#endif // GRAPENGINE_GE_IMGUI_LAYER_HPP
