#version 330 core

// attributes
layout(location = 0) out vec4 color;
// pipeline variables
out vec3 v_Pos_ws;
out vec3 v_LightPos_ws;
out vec3 v_LightDir_cs;
out vec3 v_EyeDir_cs;
out vec3 v_Normal_cs;
// uniforms
uniform float u_LightPower;
uniform vec3  u_LightColor;
uniform vec3  u_AmbientColor;
uniform vec3  u_DiffuseColor;
uniform vec3  u_SpecularColor;

void main() {
  float dist = length(v_LightPos_ws-v_Pos_ws);
  // Normal of the computed fragment, in camera space
  vec3 n = normalize(v_Normal_cs);
  // Direction of the light (from the fragment to the light)
  vec3 l = normalize(v_LightDir_cs);
  float cosTheta = clamp(dot(n, l), 0, 1);

	// Eye vector (towards the camera)
	vec3 e = normalize(v_EyeDir_cs);
	// Direction in which the surface reflects the light
	vec3 r = reflect(-l,n);
	float cosAlpha = clamp(dot(e, r), 0, 1);
	
	vec3 color3 =
		// Ambient : simulates indirect lighting
		u_AmbientColor +
		// Diffuse : "color" of the object
		u_DiffuseColor*u_LightColor*u_LightPower*cosTheta/(dist*dist) +
		// Specular : reflective highlight, like a mirror
		u_SpecularColor*u_LightColor*u_LightPower*pow(cosAlpha,5)/(dist*dist);
  //color = vec4(color3, 1);
  color = vec4(
    abs(v_Normal_cs.x),
    abs(v_Normal_cs.y),
    abs(v_Normal_cs.z),
    1.0
  );
}
