#include "renderer.h"
#include "../models/obj-parser.h"
#include "../errors.h"

ModelRenderer::ModelRenderer(
  Window *win,
  const std::string objFile,
  const bool computeNormals
) : m_win{ win }, m_objFile{ objFile } {
  ObjParser parser;
  m_model = parser.parse(objFile);
  if (parser.isParseError()) {
    m_isError = true;
    m_errorMsg = parser.getParseError();
  }
  // compute normals if the object file did not provide any or if requested
  if (computeNormals && m_model.normals.size() == 0)
    m_model.computeNormals();
}

int ModelRenderer::aggregateVertexData(
  VertexArray *va,
  VertexBuffer *vb,
  bool *usingTex
) {
  // determine which data to aggregate
  // we ALWAYS have normal and vertex position data, so we just have to check
  // if there are texture coordinates provided
  //
  // the (possibly large) aggregatedData vector is freed immediately after
  // return since it goes out of scope (and the data has been copied over to GPU
  // buffer).
  va->Bind();
  vb->Bind();
  if (m_model.texCoords.size() != 0) {
    auto aggregatedData = aggregatePosTexNor(m_model);
    m_log << LoggerState::Info << "aggregated position, texture & normal data\n";
    vb->setBufferData(
      (void *)aggregatedData.data(),
      (unsigned int)(aggregatedData.size() * sizeof(AggVertex_pos_tex_nor))
    );
    m_log << LoggerState::Info << "copied vertex data to GPU buffer\n";
    va->AddBuffer(*vb, AggVertex_pos_tex_nor::getLayout());
    m_log << LoggerState::Info << "setup vertex buffer layout\n";
    *usingTex = true;
    return aggregatedData.size();
  }
  auto aggregatedData = aggregatePosNor(m_model);
  m_log << LoggerState::Info << "aggregated position & normal data\n";
  vb->setBufferData(
    (void *)aggregatedData.data(),
    (unsigned int)(aggregatedData.size() * sizeof(AggVertex_pos_nor))
  );
  m_log << LoggerState::Info << "copied vertex data to GPU buffer\n";
  va->AddBuffer(*vb, AggVertex_pos_nor::getLayout());
  m_log << LoggerState::Info << "setup vertex buffer layout\n";
  *usingTex = false;
  return aggregatedData.size();
}


/* =============================================================================
 * UNIFORM SETUP
 * =============================================================================
 */

// bits indicating which uniforms to set for the shader
static const char B_UNIF_TRANSFORM_MATS = (0x1 << 0x0); // set transformation matrices
static const char B_UNIF_CONSTANTS      = (0x1 << 0x1); // set all values which never change
// names of the uniforms to be used in the shader
static const char *UNIF_MODEL_MATRIX     = "u_MatModel";
static const char *UNIF_VIEW_MATRIX      = "u_MatView";
static const char *UNIF_PROJ_MATRIX      = "u_MatProj";
static const char *UNIF_LIGHT_POWER      = "u_LightPower";
static const char *UNIF_LIGHT_COLOR      = "u_LightColor";
static const char *UNIF_LIGHT_POS_MS     = "u_LightPos_ms";
static const char *UNIF_CAMERA_ORIGIN_MS = "u_CameraOrigin_ms";

void ModelRenderer::setInitialUniforms(RenderCtx *ctx, const char uniformBits) {
  if (!ctx->sh) return;
  ctx->sh->Bind();
  if (uniformBits & B_UNIF_TRANSFORM_MATS) {
    auto u_MatModel = MatrixUniform<glm::mat4>{
      UNIF_MODEL_MATRIX, 4, *ctx->mat_model
    };
    auto u_MatView = MatrixUniform<glm::mat4>{
      UNIF_VIEW_MATRIX, 4, *ctx->mat_view
    };
    auto u_MatProj = MatrixUniform<glm::mat4>{
      UNIF_PROJ_MATRIX, 4, *ctx->mat_proj
    };
    ctx->sh->setUniforms({ &u_MatModel, &u_MatView, &u_MatProj });
    m_log << LoggerState::Info << "set uniforms: transformation matrices\n";
  }
  if (uniformBits & B_UNIF_CONSTANTS) {
    // first set the renderer specific uniforms
    auto u_LightPower = VectorUniform<float>{
      UNIF_LIGHT_POWER, 1, ctx->lightPower
    };
    auto u_LightColor = VectorUniform<float>{
      UNIF_LIGHT_COLOR, 3,
      ctx->lightColor->r, ctx->lightColor->g, ctx->lightColor->b
    };
    auto u_LightPos_ms = VectorUniform<float>{
      UNIF_LIGHT_POS_MS, 3,
      ctx->lightPos->x, ctx->lightPos->y, ctx->lightPos->z
    };
    auto u_CameraOrigin_ms = VectorUniform<float>{
      UNIF_CAMERA_ORIGIN_MS, 3,
      ctx->cameraOrigin->x, ctx->cameraOrigin->y, ctx->cameraOrigin->z
    };
    ctx->sh->setUniforms({
      &u_LightPower, &u_LightColor, &u_LightPos_ms, &u_CameraOrigin_ms,
    });
    m_log << LoggerState::Info << "set uniforms: renderer specific\n";
    // then set the model specific uniforms
    if (ctx->material) {
      auto materialUniforms = ctx->material->compileUniforms(MAT_ATTR_ALL);
      for (auto & unif : materialUniforms) ctx->sh->setUniform(&unif);
      m_log << LoggerState::Info << "set uniforms: material uniforms\n";
    }
  }
}

/* =============================================================================
 * INPUT HANDLING
 * =============================================================================
 */

// rotation handler - updates the model matrix uniform in the shader so that the
// object is rotated
static KeyEventHandler rotationHandler =
  [](void *context, int key, int scancode, int action, int mods) {
    if (action == ActionRelease) return;
    RenderCtx *ctx = (RenderCtx *)context;
    switch (key) {
      case KeyX: { ctx->curr_axis = 'x'; return; }
      case KeyY: { ctx->curr_axis = 'y'; return; }
      case KeyZ: { ctx->curr_axis = 'z'; return; }
    }
    float d_rot;
    glm::vec3 axis{ 0.0f, 0.0f, 0.0f };
    switch (ctx->curr_axis) {
      case 'x': { d_rot = ctx->dx_rot; axis.x = 1.0f; break; }
      case 'y': { d_rot = ctx->dy_rot; axis.y = 1.0f; break; }
      case 'z': { d_rot = ctx->dz_rot; axis.z = 1.0f; break; }
      default:  { return; }
    }
    if      (key == KeyRight) {}
    else if (key == KeyLeft)  { d_rot *= -1; }
    else return;
    *ctx->mat_model = glm::rotate(*ctx->mat_model, glm::radians(d_rot), axis);
    MatrixUniform<glm::mat4> u_MatModel{ UNIF_MODEL_MATRIX, 4, *ctx->mat_model };
    ctx->sh->Bind();
    ctx->sh->setUniform(&u_MatModel);
    GLCheckError("rotation handler uniform updates");
  };

// zoom handler - updates the model matrix uniform in the shader so that the
// object is scaled accordingly
static CharEventHandler zoomHandler =
  [](void *context, unsigned int codepoint) {
    RenderCtx *ctx = (RenderCtx *)context;
    float scale = ctx->d_scale;
    if      (codepoint == '+') { scale = 1 + scale; }
    else if (codepoint == '-') { scale = 1 - scale; }
    else return;
    *ctx->mat_model = glm::scale(*ctx->mat_model, {scale, scale, scale});
    MatrixUniform<glm::mat4> u_MatModel{ UNIF_MODEL_MATRIX, 4, *ctx->mat_model };
    ctx->sh->Bind();
    ctx->sh->setUniform(&u_MatModel);
    GLCheckError("zoom handler uniform updates");
  };

/* =============================================================================
 * RENDERING
 * =============================================================================
 */

void ModelRenderer::render() {
  VertexArray  va;
  VertexBuffer vb;
  m_log << LoggerState::Info << "aggregating model vertices...\n";
  bool usingTex{ false };
  int vertexCount = aggregateVertexData(&va, &vb, &usingTex);
  // need to load texture if using them
  Texture   diffuseTx,
            specularTx;
  Material *material{ nullptr };
  // default shaders for when there is no texture
  std::string vertexShaderPath  { "resources/shaders/basic-vertex-shader.glsl"   },
              fragmentShaderPath{ "resources/shaders/basic-fragment-shader.glsl" };
  if (usingTex) {
    // when using textures we need a named material;
    // if a material is named, it exists since this is checked while parsing
    if (m_model.material == "") {
      m_log << LoggerState::Error
            << "have texture coordinates, but no named material - exiting\n";
      exit(1);
    }
    // load the texture;
    // there must be one since texture coordinates have been specified
    for (Material & mat : m_model.materialPalette)
      if (mat.name == m_model.material) material = &mat;
    if (material->diffuseTxMap == ""){
      m_log << LoggerState::Error
            << "have texture coordinates, but no diffuse texture - exiting\n";
      exit(1);
    }
    diffuseTx.initialize(material->diffuseTxMap);
    if (material->specularTxMap != "") {
      specularTx.initialize(material->specularTxMap);
    }
  }

  Shader sh{{ vertexShaderPath, fragmentShaderPath }};

  float       lightPower   { 50.0f            };
  rgb_color_t lightColor   { 1.0f, 1.0f, 1.0f };
  glm::vec3   lightPos     { 1.0f, 1.0f, 1.0f };
  glm::vec3   worldOrigin  { 0.0f, 0.0f, 0.0f };
  glm::vec3   cameraOrigin {
    m_model.boundingBox.x_max + 2.0f,
    m_model.boundingBox.y_max + 2.0f,
    m_model.boundingBox.z_max + 2.0f
  };
  // transformation matrices
  auto winDim = m_win->getDimensions();
  float aspectRatio{ (float)winDim.width/(float)winDim.height };
  glm::mat4
    matModel{
      glm::scale(glm::mat4(1.0f), glm::vec3(0.5f))
    },
    matView{
      glm::lookAt(cameraOrigin, worldOrigin, {0,1,0})
    },
    matProj{
      glm::perspective(
        aspectRatio, glm::radians(45.0f), 1.0f, 100.0f
      )
    };
  // TODO correct aspect ratio and 

  Renderer renderer;
  RenderCtx ctx {
    usingTex,
    &renderer,
    &va,
    &vb,
    diffuseTx.isInitialized()  ? &diffuseTx  : nullptr,
    specularTx.isInitialized() ? &specularTx : nullptr,
    &sh,
    lightPower,
    material,
    material ? &material->ambientColor  : nullptr,
    material ? &material->diffuseColor  : nullptr,
    material ? &material->specularColor : nullptr,
    &lightColor,
    &matModel,
    &matView,
    &matProj,
    &cameraOrigin,
    &worldOrigin,
    &lightPos,
    // renderer state related to input
    1.0f, // dx_rot
    1.0f, // dy_rot
    1.0f, // dz_rot
    'z',  // curr_axis
    0.1f  // d_scale
  };

  setInitialUniforms(&ctx, B_UNIF_CONSTANTS | B_UNIF_TRANSFORM_MATS);
  // setup input controller
  InputController ic;
  ic.setHandleKeyEvents(true);
  ic.setHandleCharEvents(true); ic.Bind();
  ic.addKeyEventHandler(rotationHandler, (void *)(&ctx));
  ic.addCharEventHandler(zoomHandler,     (void *)(&ctx));
  m_win->setInputController(&ic);

  while (!m_win->shouldClose()) {
    renderer.Clear();
    renderer.Draw(va, vertexCount, sh);
    GLCheckError("draw call");
    // swap buffers and poll for events
    m_win->swapBuffers();
    glfwWaitEvents();
  }
}
