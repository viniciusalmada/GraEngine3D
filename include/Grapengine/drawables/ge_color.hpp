#ifndef GRAPENGINE_COLOR_HPP
#define GRAPENGINE_COLOR_HPP

#include "math/ge_vector.hpp"

namespace GE
{
  struct Color
  {
    u8 R = 0x00;
    u8 G = 0x00;
    u8 B = 0x00;
    u8 A = 0xFF;

    constexpr explicit Color(u32 rgba)
    {
      R = static_cast<u8>((rgba >> 8 * 3) & 0xFF);
      G = static_cast<u8>((rgba >> 8 * 2) & 0xFF);
      B = static_cast<u8>((rgba >> 8 * 1) & 0xFF);
      A = static_cast<u8>((rgba >> 8 * 0) & 0xFF);
    }

    explicit Color(Vec3 rgb)
    {
      R = static_cast<u8>(rgb.x * 255.0f);
      G = static_cast<u8>(rgb.y * 255.0f);
      B = static_cast<u8>(rgb.z * 255.0f);
    }

    Vec4 ToVec4() const
    {
      return {
        (f32)R / 255.0f,
        (f32)G / 255.0f,
        (f32)B / 255.0f,
        (f32)A / 255.0f,
      };
    }

    Vec3 ToVec3() const
    {
      return {
        (f32)R / 255.0f,
        (f32)G / 255.0f,
        (f32)B / 255.0f,
      };
    }
  };

  namespace Colors
  {
    constexpr Color BLACK{ 0x000000FF };
    constexpr Color WHITE{ 0xFFFFFFFF };
    constexpr Color RED{ 0xFF3333FF };
    constexpr Color BLUE{ 0x3333FFFF };
    constexpr Color GREEN{ 0x33FF33FF };
    constexpr Color MAGENTA{ 0xFF33FFFF };
  }
}

#endif // GRAPENGINE_COLOR_HPP
