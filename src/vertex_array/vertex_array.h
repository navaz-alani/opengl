#ifndef __VERTEX_ARRAY_H__
#define __VERTEX_ARRAY_H__

#include "../buffer/vertex_buffer.h"
#include "../buffer/layout.h"

class VertexArray {
  unsigned int m_glID;

public:
  VertexArray();
  ~VertexArray();

  void Bind() const;
  void Unbind() const;

  void AddBuffer(const VertexBuffer &vb, const BufferLayout &layout);
};

#endif
