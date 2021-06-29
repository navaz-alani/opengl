#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Pos;

void main() {
  float x = abs(v_Pos.x);
  float y = abs(v_Pos.y);
  float z = abs(v_Pos.z);
  color = vec4(x, y, z, 1.0);
}
