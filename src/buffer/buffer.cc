#include "buffer.h"

Buffer::Buffer(const BufferType type)
  : m_type{ type } {
  glGenBuffers(1, &m_glID);
  glBindBuffer(GLBuffType(m_type), m_glID);
}

Buffer::~Buffer() {
  glBindBuffer(GLBuffType(m_type), 0);
  glDeleteBuffers(1, &m_glID);
}

unsigned int Buffer::glID() const {
  return m_glID;
}

void Buffer::Bind() const {
  glBindBuffer(GLBuffType(m_type), m_glID);
}

void Buffer::Unbind() const {
  glBindBuffer(GLBuffType(m_type), 0);
}
