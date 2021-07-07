#ifndef __MODEL_RENDERER_H__
#define __MODEL_RENDERER_H__

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../logger/logger.h"
#include "../models/object.h"
#include "../window/window.h"
#include "../buffer/vertex_buffer.h"
#include "../vertex_array/vertex_array.h"
#include "../textures/texture.h"
#include "../shader/shader.h"
#include "../shader/matrix_uniform.h"
#include "../shader/vector_uniform.h"
#include "../renderer/renderer.h"
#include "../models/material.h"

struct RenderCtx {
  bool          usingTex;
  Renderer     *renderer;
  VertexArray  *va;
  VertexBuffer *vb;
  Texture      *diffuseTx,
               *specularTx;
  Shader       *sh;
  // renderer uniforms
  float         lightPower;
  // read in from material definition
  Material     *material;
  rgb_color_t  *ambientColor,
               *diffuseColor,
               *specularColor,
  // set by renderer
               *lightColor;
  // subject to change (based on input)
  glm::mat4    *mat_model,
               *mat_view,
               *mat_proj;
  glm::vec3    *cameraOrigin,
               *worldOrigin,
               *lightPos;
  // renderer input-modifiable state 
  // rotation
  float x_rot, dx_rot,
        y_rot, dy_rot,
        z_rot, dz_rot;
  char  curr_axis;
  // scaling - uniform along all axes
  float scale, d_scale;
};

// ModelRenderer is an object which renders a 3D object inputted through an
// object file (.obj extension) which contains object data specified as per the
// Wavefront specifications. Material files (.mtl extension) linked in the
// object file are also parsed and used in the render process.
//
// The renderer's input controller allows the object to be explored by rotating
// it using the keyboard (TODO rotation controls).
//
// The object makes decisions on how to render the object using the data
// provided in the object file (and all linked material files).
// For example, if normal data is not provided, it is (by default) computed and
// used to improve the lighting effects.
// If linked material files do not specify a texture, then the normals are used
// to color the render.
class ModelRenderer {
  bool              m_isError;
  std::string       m_errorMsg;

  Window           *m_win;
  const std::string m_objFile;
  Object3D          m_model;

  Logger            m_log;

  // aggregated the vertex data into the given vertex buffer, deciding which
  // attributes are aggregated based on the model's data.
  int aggregateVertexData(
    VertexArray *va,
    VertexBuffer *vb,
    bool *usingVertices
  );
  void setInitialUniforms(RenderCtx *ctx, const char uniformBits);
public:
  ModelRenderer(
    Window *win,
    const std::string objFile,
    const bool computeNormals = true
  );
  ~ModelRenderer() = default;

  void render();

  // error checking

  inline bool isError() const { return m_isError; }
  inline const std::string &errMsg() const { return m_errorMsg; }
};

#endif
