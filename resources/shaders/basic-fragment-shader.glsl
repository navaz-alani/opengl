#version 330 core

layout(location = 0) out vec4 color;

void main() {
  float x;
  if (gl_PointCoord.x < 0) x = -1*gl_PointCoord.x;
  else x = gl_PointCoord.x;
  float y;
  if (gl_PointCoord.y < 0) y = -1*gl_PointCoord.y;
  else y = gl_PointCoord.y;

  color = vec4(x, y, 0.0, 1.0);
}
