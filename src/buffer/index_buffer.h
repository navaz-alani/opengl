#ifndef __INDEX_BUFFER_H__
#define __INDEX_BUFFER_H__

#include "buffer.h"

class IndexBuffer : public Buffer {
  unsigned int m_count;
public:
  IndexBuffer(const unsigned int *indices, unsigned int count);
  ~IndexBuffer() = default;

  inline unsigned int count() const { return m_count; }
};

#endif
