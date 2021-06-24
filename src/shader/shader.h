#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>

#include "../errors.h"
#include "loader.h"
#include "vector_uniform.h"

class Shader {
  unsigned int m_glID;
public:
  Shader(const ShaderSource src);
  ~Shader();

  void Bind() const;
  void Unbind() const;

  template<typename T>
  void setVectorUniform(const std::string name, VectorUniform<T> *u) {
    // TODO: uniform location cache

    int location = glGetUniformLocation(m_glID, name.c_str());
    if (location == -1) return;
    u->setToLocation(location);
  }
};

#endif
