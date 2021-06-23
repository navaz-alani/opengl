#include "layout.h"

BufferLayout::BufferLayout()
  : m_stride{ 0 }
{}

void BufferLayout::PushField(unsigned int count) {
  m_elements.push_back({ ElementType::Float, count, false });
  m_stride += sizeofElementType(ElementType::Float) * count;
}

void BufferLayout::PushField(ElementType t, unsigned int count) {
  m_elements.push_back({ t, count, false });
  m_stride += sizeofElementType(t) * count;
}

void BufferLayout::PushField(ElementType t, unsigned int count, bool normalized) {
  m_elements.push_back({ t, count, normalized });
  m_stride += sizeofElementType(t) * count;
}
