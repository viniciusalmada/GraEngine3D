#ifndef GRAPENGINE_GE_FRAMEBUFFER_HPP
#define GRAPENGINE_GE_FRAMEBUFFER_HPP

#include "ge_renderer_id.hpp"
#include "math/ge_vector.hpp"
#include "utils/ge_dimension.hpp"

namespace GE
{
  class Framebuffer
  {
  public:
    static Ptr<Framebuffer> Make(const Dimension& dimension);

    explicit Framebuffer(const Dimension& dimension);
    ~Framebuffer();

    void Invalidate();
    void Resize(Dimension dim);

    void Bind() const;
    void Unbind() const;

    //    [[nodiscard]] const FBSpecs& GetSpec() const;

    [[nodiscard]] RendererID GetColorAttachmentID() const;

    [[nodiscard]] const Dimension& GetDimension() const;

  private:
    void Clear();

    Dimension m_dimension;
    //    u32 samples = 1;
    //    bool swap_chain_target = false;
    RendererID m_id;
    RendererID m_color_attachment;
    RendererID m_depth_attachment;
  };
}

#endif // GRAPENGINE_GE_FRAMEBUFFER_HPP
