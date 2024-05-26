#include "drawables/ge_world_reference.hpp"

#include "profiling/ge_profiler.hpp"

#include <drawables/ge_cube.hpp>
#include <drawables/ge_cylinder.hpp>
#include <renderer/ge_texture_2d.hpp>

using namespace GE;

namespace
{
  constexpr Vec3 ORIGIN{ 0, 0, 0 };
  constexpr f32 AXIS_RADIUS = 0.02F;
  constexpr Color PLATFORM_COLOR{ 0xAAAAAAFF };
  constexpr auto PLATFORM_MULTIPLIER = 10;
}

GE::WorldReference::WorldReference(const Ptr<IShaderProgram>& shader, u64 platformSize) :
    m_shader(shader), m_platform_side_size(platformSize)
{
  GE_PROFILE;
  const f32 SIDE_SIZE = static_cast<f32>(m_platform_side_size);
  m_blank_texture = MakeRef<Texture2D>();
  m_platform = MakeRef<Cube>(PLATFORM_COLOR, m_shader, m_blank_texture);
  m_platform->SetScale(SIDE_SIZE, AXIS_RADIUS, SIDE_SIZE);
  m_platform->SetTranslate(SIDE_SIZE / 2, 0, SIDE_SIZE / 2);

  m_x_axis = Cylinder::Make(m_shader,
                            ORIGIN,
                            AXIS_RADIUS,
                            Vec3{ 1, 0, 0 },
                            SIDE_SIZE * PLATFORM_MULTIPLIER,
                            Colors::RED,
                            m_blank_texture);
  m_y_axis = Cylinder::Make(m_shader,
                            ORIGIN,
                            AXIS_RADIUS,
                            Vec3{ 0, 1, 0 },
                            SIDE_SIZE * PLATFORM_MULTIPLIER,
                            Colors::GREEN,
                            m_blank_texture);
  m_z_axis = Cylinder::Make(m_shader,
                            ORIGIN,
                            AXIS_RADIUS,
                            Vec3{ 0, 0, 1 },
                            SIDE_SIZE * PLATFORM_MULTIPLIER,
                            Colors::BLUE,
                            m_blank_texture);
}

void WorldReference::DrawBatch() const
{
  GE_PROFILE;
  m_shader->Activate();

  if (m_show_platform)
    m_platform->Draw();
  m_x_axis->Draw();
  m_y_axis->Draw();
  m_z_axis->Draw();
}

void GE::WorldReference::ShowPlatform(bool show)
{
  m_show_platform = show;
}

Ptr<DrawingObject> GE::WorldReference::GetVAO() const
{
  return m_platform->GetVAO();
}

Mat4 GE::WorldReference::GetModelMatrix() const
{
  return m_platform->GetModelMatrix();
}

WorldReference::~WorldReference() = default;
