#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec4 v_Pos;
out vec3 v_Normal;

uniform mat4 u_MVP;

void main() {
  gl_Position = u_MVP * position;
  v_Pos = gl_Position;
  v_Normal = normal;
}
