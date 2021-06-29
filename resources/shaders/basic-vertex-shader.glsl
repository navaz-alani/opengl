#version 330 core

layout(location = 0) in vec4 position;

out vec4 v_Pos;

uniform mat4 u_MVP;

void main() {
  gl_Position = u_MVP * position;
  v_Pos = gl_Position;
}
