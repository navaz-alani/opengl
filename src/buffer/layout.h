#ifndef __LAYOUT_H__
#define __LAYOUT_H__

#include <vector>
#include <map>

#include <GL/glew.h>

// OpenGL supported element types
enum class ElementType: unsigned int {
  Byte   = GL_BYTE,
  UByte  = GL_UNSIGNED_BYTE,
  Short  = GL_SHORT,
  UShort = GL_UNSIGNED_SHORT,
  Int    = GL_INT,
  UInt   = GL_UNSIGNED_INT,
  Float  = GL_FLOAT,
  Double = GL_DOUBLE
};

static std::map<ElementType,unsigned int> ElementTypeSizes = {
  { ElementType::Byte,   sizeof(char) },
  { ElementType::UByte,  sizeof(unsigned char) },
  { ElementType::Short,  sizeof(short) },
  { ElementType::UShort, sizeof(unsigned short) },
  { ElementType::Int,    sizeof(int) },
  { ElementType::UInt,   sizeof(unsigned int) },
  { ElementType::Float,  sizeof(float) },
  { ElementType::Double, sizeof(double) },
};

unsigned int inline sizeofElementType(const ElementType e) {
  return ElementTypeSizes[e];
}

unsigned int inline GLElementType(const ElementType e) {
  return static_cast<unsigned int>(e);
}

struct LayoutElement {
  ElementType  m_type;     // type of the field's elements
  unsigned int m_count;    // number of elements in this field (1, 2, 3, 4)
  bool         normalized; // whether the floats are normalized
};

class BufferLayout {
  std::vector<LayoutElement> m_elements;
  unsigned int               m_stride;

public:
  BufferLayout();
  ~BufferLayout() = default;

  // deafults element type to ElementType::Float and normalized to false
  void PushField(unsigned int count);
  // deafults normalized to false
  void PushField(ElementType t, unsigned int count);
  void PushField(ElementType t, unsigned int count, bool normalized);

  inline unsigned int stride() const { return m_stride; }
  inline const std::vector<LayoutElement> &fields() const { return m_elements; }
};

#endif
