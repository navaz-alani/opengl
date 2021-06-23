#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#ifndef __SHADER_LOADER_H__
#define __SHADER_LOADER_H__

#include <string>

#include "../status/status.h"

struct LoaderStatus : public Status {
  unsigned int assetID;
  ~LoaderStatus() = default;
};

// ShaderLoader takes names of files corresponding to vertex and fragment
// shaders, compiles them and links them into a new program.
class ShaderLoader {
  const std::string m_vertexShaderFile;
  const std::string m_fragShaderFile;

public:
  ShaderLoader(const std::string vertexShaderFile, const std::string fragShaderFile);

  // CreateProgram compiles and links the vertex and fragment shaders into a new
  // openGL program object, whose handle is stored in the `assetID` field of the
  // returned LoaderStatus object.
  LoaderStatus CreateProgram();
};

#endif
