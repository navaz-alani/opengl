#include "object.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Object3D::computeNormals() {
  // make sure we are dealing with triangles
  assert(!(vertexIndices.size() % 3));
  normals.clear(); normals.resize(vertexPositions.size());
  // first compute all normals for all faces
  position_3d_t v0, v1, v2, lineVec1, lineVec2;
  v_normal_t normal;
  for (unsigned int i = 0; i < vertexIndices.size(); i+=3) {
    normal = { 0.0f, 0.0f, 0.0f };
    v0 = vertexPositions[vertexIndices[i]];
    v1 = vertexPositions[vertexIndices[i+1]];
    v2 = vertexPositions[vertexIndices[i+2]];
    lineVec1 = v1 - v0;
    lineVec2 = v2 - v0;
    normal = (lineVec1*lineVec2);
    normals[vertexIndices[i]]   += normal;
    normals[vertexIndices[i+1]] += normal;
    normals[vertexIndices[i+2]] += normal;
  }
  for (auto &normal : normals) { normal.normalize(); }
  normalIndices = vertexIndices;
}

template<typename T>
inline T accessIdx(const std::vector<T> &vec, unsigned int idx) {
  return (idx >= vec.size()) ? T() : vec[idx];
}

/*
 * AggVertex_pos_tex implementation
 */

BufferLayout AggVertex_pos_tex::getLayout() {
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
  unsigned int vIdx, uvIdx;
  for (unsigned int i = 0; i < obj.vertexIndices.size(); ++i) {
    vIdx = accessIdx(obj.vertexIndices, i);
    uvIdx = accessIdx(obj.texIndices, i);
    aggregated.push_back({
      accessIdx(obj.vertexPositions, vIdx),
      accessIdx(obj.texCoords, uvIdx)
    });
  }
  return aggregated;
}

/*
 * AggVertex_pos_nor implementation
 */

BufferLayout AggVertex_pos_nor::getLayout() {
  BufferLayout layout;
  layout.PushField(3); // position
  layout.PushField(3); // normal vector
  return layout;
}

AggVertex_pos_nor::AggVertex_pos_nor(position_3d_t pos, v_normal_t nor)
  : m_pos{ pos }, m_nor{ nor }
{ }

std::vector<AggVertex_pos_nor> aggregatePosNor(Object3D &obj) {
  assert(obj.vertexIndices.size() == obj.normalIndices.size());
  std::vector<AggVertex_pos_nor> aggregated;
  unsigned int vIdx, nIdx;
  for (unsigned int i = 0; i < obj.vertexIndices.size(); ++i) {
    vIdx = accessIdx(obj.vertexIndices, i);
    nIdx = accessIdx(obj.normalIndices, i);
    aggregated.push_back({
      accessIdx(obj.vertexPositions, vIdx),
      accessIdx(obj.normals, nIdx)
    });
  }
  return aggregated;
}

/*
 * AggVertex_pos_tex_nor implementation
 */

BufferLayout AggVertex_pos_tex_nor::getLayout() {
  BufferLayout layout;
  layout.PushField(3); // position
  layout.PushField(2); // texture coords
  layout.PushField(3); // normal
  return layout;
}

AggVertex_pos_tex_nor::AggVertex_pos_tex_nor(
  position_3d_t pos, tex_2d_t tex, v_normal_t nor
) : m_pos{ pos }, m_tex{ tex }, m_nor{ nor }
{ }

std::vector<AggVertex_pos_tex_nor> aggregatePosTexNor(Object3D &obj) {
  assert(obj.vertexIndices.size() == obj.normalIndices.size() == obj.texIndices.size());
  std::vector<AggVertex_pos_tex_nor> aggregated;
  unsigned int vIdx, uvIdx, nIdx;
  for (unsigned int i = 0; i < obj.vertexIndices.size(); ++i) {
    vIdx = obj.vertexIndices[i];
    uvIdx = accessIdx<unsigned int>(obj.texIndices, i);
    nIdx = obj.normalIndices[i];
    aggregated.push_back({
      obj.vertexPositions[vIdx],
      accessIdx<tex_2d_t>(obj.texCoords, uvIdx),
      obj.normals[nIdx]
    });
  }
  return aggregated;
}
