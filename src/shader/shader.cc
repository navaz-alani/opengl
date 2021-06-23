#include "shader.h"

#include <GL/glew.h>

Shader::Shader(const ShaderSource src) {
  ShaderLoader loader{ src };
  LoaderStatus stat = loader.CreateProgram();
  stat.log(true); // if shader source fails to load, we exit!
  m_glID = stat.assetID;
}

Shader::~Shader() { glDeleteProgram(m_glID); }

void Shader::Bind() const { glUseProgram(m_glID); }

void Shader::Unbind() const { glUseProgram(0); }
