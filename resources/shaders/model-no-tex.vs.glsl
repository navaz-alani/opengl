#version 330 core

// DEFINITIONS
// ms -> model space  (specified vertex coordinates)
// ws -> world space  (model space coordinates, transformed | u_MatModel)
// cs -> camera space (word space coordinates, adjusted for camera | u_MatView)
//
// NOTES
// - u_MatModel MUST scale model by same factor in all directions

// attributes
layout(location = 0) in vec3 a_Pos_ms;    // model space position
layout(location = 1) in vec3 a_Normal;    // model space surface normal
// pipeline variables
out vec3 v_Pos_ws;
out vec3 v_LightPos_ws;
out vec3 v_LightDir_cs;
out vec3 v_EyeDir_cs;
out vec3 v_Normal_cs;
// uniforms
uniform mat4 u_MatModel;
uniform mat4 u_MatView;
uniform mat4 u_MatProj;
uniform vec3 u_LightPos_ms;
uniform vec3 u_CameraOrigin_ms;

void main() {
  gl_Position = u_MatProj*u_MatView*u_MatModel*vec4(a_Pos_ms, 1);
  v_Pos_ws = (u_MatModel*vec4(a_Pos_ms,1)).xyz;
  // eye direction in camera space
  vec3 pos_cs = (u_MatView*u_MatModel*vec4(a_Pos_ms,1)).xyz;
  v_EyeDir_cs = (u_MatView*u_MatModel*vec4(u_CameraOrigin_ms,1)).xyz - pos_cs;
  // light direction, camera space
  v_LightPos_ws = (u_MatModel*vec4(u_LightPos_ms,1)).xyz;
  vec3 lightPos_cs = (u_MatView*u_MatModel*vec4(u_LightPos_ms,1)).xyz;
  v_LightDir_cs = lightPos_cs + v_EyeDir_cs;
  // surface normal in camera space
  //v_Normal_cs = (u_MatView*u_MatModel*vec4(a_Normal,0)).xyz;
  v_Normal_cs = a_Normal;
}
