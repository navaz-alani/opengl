#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Pos;
in vec3 v_Normal;

void main() {
  color = vec4(abs(v_Normal.x), abs(v_Normal.y), abs(v_Normal.z), 1.0);
}
