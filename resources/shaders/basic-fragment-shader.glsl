#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Pos;

void main() {
  float x = v_Pos.x, y = v_Pos.y, z = v_Pos.z;
  if (v_Pos.x < 0) x = -1 * v_Pos.x;
  if (v_Pos.y < 0) y = -1 * v_Pos.y;
  if (v_Pos.z < 0) z = -1 * v_Pos.z;
  color = vec4(x, y, z, 1.0);
}
