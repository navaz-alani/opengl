#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

out vec4 v_Pos;
out vec3 v_Normal;

uniform mat4 u_MatModel;
uniform mat4 u_MatView;
uniform mat4 u_MatProj;

void main() {
  gl_Position = u_MatProj * u_MatView * u_MatModel * position;
  v_Pos = gl_Position;
  v_Normal = normal;
}
