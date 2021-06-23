#ifndef __INDEX_BUFFER_H__
#define __INDEX_BUFFER_H__

#include "buffer.h"

class IndexBuffer : Buffer {
public:
  IndexBuffer(const unsigned int *indices, unsigned int count);
  ~IndexBuffer() = default;
};

#endif
