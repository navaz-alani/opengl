#include "index_buffer.h"

#include <GL/glew.h>

IndexBuffer::IndexBuffer(const unsigned int *indices, unsigned int count)
  : Buffer(BufferType::IndexBuffer), m_count{ count } {
  if (!glID()) {/* handle error */ }
  glBufferData(
    GLBuffType(BufferType::IndexBuffer),
    count * sizeof(unsigned int),
    indices,
    GL_STATIC_DRAW
  );
}
