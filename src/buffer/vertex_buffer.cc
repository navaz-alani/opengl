#include "vertex_buffer.h"

#include <GL/glew.h>

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
  : Buffer(BufferType::VertexBuffer) {
  if (!glID()) {/* handle error */ }
  glBufferData(
    GLBuffType(BufferType::VertexBuffer),
    size,
    data,
    GL_STATIC_DRAW
  );
}
