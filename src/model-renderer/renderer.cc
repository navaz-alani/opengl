#include "renderer.h"
#include "../models/obj-parser.h"
#include "../errors.h"

ModelRenderer::ModelRenderer(
  const Window *win,
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
  if (m_model.texCoords.size() != 0) {
    auto aggregatedData = aggregatePosTexNor(m_model);
    m_log << LoggerState::Info << "aggregated position, texture & normal data\n";
    vb->setBufferData(
      (void *)aggregatedData.data(),
      (unsigned int)(aggregatedData.size() * sizeof(AggVertex_pos_tex_nor))
    );
    m_log << LoggerState::Info << "copied vertex data to GPU buffer\n";
    va->AddBuffer(*vb, aggregatedData[0].getLayout());
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
  va->AddBuffer(*vb, aggregatedData[0].getLayout());
  m_log << LoggerState::Info << "setup vertex buffer layout\n";
  *usingTex = false;
  return aggregatedData.size();
}


/* =============================================================================
 * RENDERING
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
    m_model.boundingBox.x_max,
    m_model.boundingBox.y_max,
    m_model.boundingBox.z_max
  };
  // transformation matrices
  glm::mat4
    matModel{
      glm::mat4()
    },
    matView{
      glm::lookAt(cameraOrigin, worldOrigin, {0,0,1})
    },
    matProj{
      glm::ortho(
        m_model.boundingBox.x_min - 2.0f,
        m_model.boundingBox.x_max + 2.0f,
        m_model.boundingBox.y_min - 2.0f,
        m_model.boundingBox.y_max + 2.0f,
        m_model.boundingBox.z_min - 2.0f,
        m_model.boundingBox.z_max + 2.0f
      )
    };

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
    &lightPos
  };

  setInitialUniforms(&ctx, B_UNIF_CONSTANTS | B_UNIF_TRANSFORM_MATS);

  while (!m_win->shouldClose()) {
    renderer.Clear();
    renderer.Draw(va, vertexCount, sh);
    GLCheckError("draw call");
    // swap buffers and poll for events
    m_win->swapBuffers();
    glfwWaitEvents();
  }
}
