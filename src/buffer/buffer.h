#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <GL/glew.h>

// BufferType is a collection of currently implemented buffer types.
// For compatability with OpenGL functions, the `GLBuffType` function can be
// used to convert a BufferType to a Glenum.
enum class BufferType: unsigned int {
  VertexBuffer = GL_ARRAY_BUFFER,
  IndexBuffer  = GL_ELEMENT_ARRAY_BUFFER
};

unsigned int inline GLBuffType(const BufferType t) {
  return static_cast<unsigned int>(t);
}

// Buffer is a class which abstracts creation and deletion of OpenGL buffers.
// Upon construction, a new OpenGL buffer is generated and upon destruction,
// said buffer is destroyed.
// During construction, the buffer is also bound to for the given BufferType and
// during destruction, the buffer is unbound.
// To detect any errors during creation of the buffer, one ought to check if the
// generated buffer's OpenGL hanlde (internally m_glID) is 0.
class Buffer {
  const BufferType m_type;
  unsigned int     m_glID; // OpenGL id of this buffer

public:
  Buffer(const BufferType type);
  ~Buffer();

  unsigned int glID() const;

  void Bind() const;
  void Unbind() const;
};

#endif
