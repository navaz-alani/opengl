#include "vertex_array.h"

#include <GL/glew.h>

VertexArray::VertexArray() { glGenVertexArrays(1, &m_glID); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &m_glID); }

void VertexArray::Bind() const {
  glBindVertexArray(m_glID);
}

void VertexArray::Unbind() const {
  glBindVertexArray(0);
}

void VertexArray::AddBuffer(
  const VertexBuffer &vb,
  const BufferLayout &layout,
  const unsigned int attribOffset
) {
  Bind();
  vb.Bind();
  const std::vector<LayoutElement> &fields = layout.fields();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < fields.size(); ++i) {
    LayoutElement e = fields[i];
    glVertexAttribPointer(
      i+attribOffset,
      e.m_count,
      GLElementType(e.m_type),
      e.normalized ? GL_TRUE : GL_FALSE,
      layout.stride(),
      (const void *)offset
    );
    glEnableVertexAttribArray(i+attribOffset);

    offset += e.m_count * sizeofElementType(e.m_type);
  }
}
