#ifndef GRAPENGINE_GE_MESH_HPP
#define GRAPENGINE_GE_MESH_HPP

#include "drawables/ge_cube.hpp"
#include "drawables/ge_cylinder.hpp"
#include "drawables/ge_drawable.hpp"

namespace GE
{
  struct Face
  {
    IVec3 indices;
    Vec3 center;
    Vec3 normal;
  };

  class Mesh : public Drawable
  {
  public:
    Mesh(std::string_view path, const Ptr<IShaderProgram>& shader);

    ~Mesh() override;
    void Draw() const override;

  private:
    Color m_color = Colors::MAGENTA;
    Ptr<DrawingObject> m_draw_primitive;
    Ptr<Cube> m_bbox;
    Ptr<IShaderProgram> m_shader;
    Ptr<Texture2D> m_texture;
    std::vector<Vec3> m_vertices;
    std::vector<Face> m_faces;
    std::vector<Ptr<Cylinder>> m_normals;
  };
}

#endif // GRAPENGINE_GE_MESH_HPP
