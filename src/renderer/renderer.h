#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "../vertex_array/vertex_array.h"
#include "../buffer/index_buffer.h"
#include "../shader/shader.h"
#include "../shader/uniform.h"

class Renderer {
public:
  void Clear() const;
  void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &sh) const;
};

#endif
