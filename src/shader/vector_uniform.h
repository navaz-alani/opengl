#ifndef __VECTOR_UNIFORM_H__
#define __VECTOR_UNIFORM_H__

#include <GL/glew.h>

#include "uniform.h"
#include "../errors.h"

// a generic vector (non-matrix) uniform
template<typename T>
class VectorUniform : public Uniform {
  unsigned char count;
  T v0, v1, v2, v3;

public:
  VectorUniform(const unsigned char count, T v0 = 0, T v1 = 0, T v2 = 0, T v3 = 0)
    : count{ count }, v0{ v0 }, v1{ v1 }, v2{ v2 }, v3{ v3 }
  {};

  inline void setToLocation(const int location = -1) override;
};
// all the supported scalar specializations have been defined below;
// if other types are used, we error out;
template<typename T>
inline void VectorUniform<T>::setToLocation(const int location) { assert(false); }

template<>
inline void VectorUniform<float>::setToLocation(const int location) {
  switch (count) {
    case 1: { glUniform1f(location, v0); break; }
    case 2: { glUniform2f(location, v0, v1); break; }
    case 3: { glUniform3f(location, v0, v1, v2); break; }
    case 4: { glUniform4f(location, v0, v1, v2, v3); break; }
    default: { assert(false); }
  }
}

template<>
inline void VectorUniform<unsigned int>::setToLocation(const int location) {
  switch (count) {
    case 1: { glUniform1ui(location, v0); break; }
    case 2: { glUniform2ui(location, v0, v1); break; }
    case 3: { glUniform3ui(location, v0, v1, v2); break; }
    case 4: { glUniform4ui(location, v0, v1, v2, v3); break; }
    default: { assert(false); }
  }
}

template<>
inline void VectorUniform<int>::setToLocation(const int location) {
  switch (count) {
    case 1: { glUniform1i(location, v0); break; }
    case 2: { glUniform2i(location, v0, v1); break; }
    case 3: { glUniform3i(location, v0, v1, v2); break; }
    case 4: { glUniform4i(location, v0, v1, v2, v3); break; }
    default: { assert(false); }
  }
};

#endif
