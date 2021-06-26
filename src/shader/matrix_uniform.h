#ifndef __MATRIX_UNIFORM_H__
#define __MATRIX_UNIFORM_H__

#include <iostream>
#include <GL/glew.h>

#include "uniform.h"
#include "../errors.h"

template<typename T>
class MatrixUniform : public Uniform {
  unsigned char m_rows, m_cols;
  const T      &m_data;
  void (*m_uniformSetter)(    // this is the function we use to set the uniform
    GLint  	       location,
    GLsizei  	     count,     // always 1 in this implementation
    GLboolean  	   transpose, // always GL_FALSE because using glm
    const GLfloat *value
  );

  // determine the function to call to set this uniform.
  void determineUniformSetter() {
    // we operate under the assumption that the number of rows of columns is
    // one of 2, 3 or 4.
    if (m_cols == 2) {
      switch (m_rows) {
        case 2:  { m_uniformSetter = glUniformMatrix2fv;   break; }
        case 3:  { m_uniformSetter = glUniformMatrix2x3fv; break; }
        default: { m_uniformSetter = glUniformMatrix2x4fv; }
      }
    } else if (m_cols == 3) {
      switch (m_rows) {
        case 2:  { m_uniformSetter = glUniformMatrix3x2fv; break; }
        case 3:  { m_uniformSetter = glUniformMatrix3fv;   break; }
        default: { m_uniformSetter = glUniformMatrix3x4fv; }
      }
    } else {
      switch (m_rows) {
        case 2:  { m_uniformSetter = glUniformMatrix4x2fv; break; }
        case 3:  { m_uniformSetter = glUniformMatrix4x3fv; break; }
        default: { m_uniformSetter = glUniformMatrix4fv; }
      }
    }
  }

public:
  // initialize a square matrix
  MatrixUniform(unsigned char numRowsCols, const T &data)
    : m_rows{ numRowsCols }, m_cols{ numRowsCols }, m_data{ data } {
    assert((2 <= numRowsCols && numRowsCols <= 4));
    determineUniformSetter();
  }

  // initialize a rectangular matrix
  MatrixUniform(unsigned char numRows, unsigned char numCols, const T &data)
    : m_rows{ numRows }, m_cols{ numCols }, m_data{ data } {
    assert((2 <= numRows && numRows <= 4) && (2 <= numCols && numCols <= 4));
    determineUniformSetter();
  }

  void setToLocation(const int location = -1) override {
    if (location == -1) return;
    m_uniformSetter(location, 1, GL_FALSE, &m_data[0][0]);
  }
};

#endif
