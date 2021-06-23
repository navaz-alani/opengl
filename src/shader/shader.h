#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>

#include "loader.h"

class Shader {
  unsigned int m_glID;
public:
  Shader(const ShaderSource src);
  ~Shader();

  void Bind() const;
  void Unbind() const;
};

#endif
