#ifndef __SHADER_H__
#define __SHADER_H__

#include <map>
#include <string>
#include <vector>

#include "../errors.h"
#include "loader.h"
#include "uniform.h"

class Shader {
  std::map<const char *, int> m_uniformLocationCache;
  unsigned int m_glID;

  int getUniformLocation(Uniform &u) {
    int location;
    if (!m_uniformLocationCache.count(u.name())) {
      location = glGetUniformLocation(m_glID, u.name());
      m_uniformLocationCache[u.name()] = location;
    } else {
      location = m_uniformLocationCache[u.name()];
    }
    return location;
  }
public:
  Shader(const ShaderSource src);
  ~Shader();

  void Bind() const;
  void Unbind() const;

  void setUniform(Uniform *u) {
    int location = getUniformLocation(*u);
    if (location == -1) return;
    u->setToLocation(location);
  }

  void setUniforms(std::vector<Uniform *> uniforms) {
    for (Uniform *u : uniforms) setUniform(u);
  }
};

#endif
