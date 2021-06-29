#include "object.h"

/*
 * AggVertex_pos_tex impl
 */

BufferLayout AggVertex_pos_tex::getLayout() const {
  BufferLayout layout;
  layout.PushField(3); // position
  layout.PushField(2); // texture coords
  return layout;
}

AggVertex_pos_tex::AggVertex_pos_tex(position_3d_t pos, tex_2d_t tex)
  : m_pos{ pos }, m_tex{ tex }
{ }

std::vector<AggVertex_pos_tex> aggregatePosTex(Object3D &obj) {
  assert(obj.vertexIndices.size() == obj.texIndices.size());
  std::vector<AggVertex_pos_tex> aggregated;
  for (unsigned int i = 0; i < obj.vertexIndices.size(); ++i)
    aggregated.push_back({
      std::move(obj.vertexPositions[obj.vertexIndices[i]]),
      std::move(obj.texCoords[obj.texIndices[i]])
    });
  return aggregated;
}
