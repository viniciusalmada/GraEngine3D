#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_texture_coord;
layout (location = 2) in vec4 in_color;
layout (location = 3) in vec3 in_normal;

out vec2 out_texture_coords;
out vec4 out_color;
out vec3 out_normal;
out vec3 out_frag_pos;

uniform mat4 u_M;
uniform mat4 u_VP;

void main()
{
  gl_Position = (u_VP) * u_M * vec4(in_position, 1.0);
  out_frag_pos = vec3(u_M * vec4(in_position, 1.0));
  out_texture_coords = in_texture_coord;
  out_color = in_color;
  out_normal = in_normal;
}