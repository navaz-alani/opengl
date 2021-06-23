#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

#include "buffer.h"

class VertexBuffer : public Buffer {
public:
  VertexBuffer(const void *data, unsigned int size);
  ~VertexBuffer() = default;
};

#endif
