#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "../buffer/layout.h"

// a 3D coordinate position
struct position_3d_t { float x, y, z; };
// a normal vector
struct v_normal_t    { float x, y, z; };
// vertex indices defining a 3D plane
struct face_t        { unsigned int v0, v1, v2; };
// a 2D texture coordinate
struct tex_2d_t      { float x, y; };

// A parsed 3D object.
struct Object3D {
  std::vector<position_3d_t> vertexPositions;
  std::vector<tex_2d_t>      texCoords;
  std::vector<v_normal_t>    normals;
  std::vector<unsigned int>  vertexIndices;
  std::vector<unsigned int>  texIndices;
  std::vector<unsigned int>  normalIndices;
};

/*
 * VERTEX AGGREGATION
 */

/*
// aggregate vertex is a composition of multiple vertex attributes
class AggregateVertex {
public:
  AggregateVertex() = default;
  virtual ~AggregateVertex() = default;

  virtual BufferLayout getLayout() const = 0;
};
*/

// vertex with two attributes - position and texture coordinates
struct AggVertex_pos_tex {
public:
  position_3d_t m_pos;
  tex_2d_t      m_tex;

  AggVertex_pos_tex(position_3d_t pos, tex_2d_t tex);
  ~AggVertex_pos_tex() = default;

  BufferLayout getLayout() const;
};

// aggregate vertex position and texture coordinate  vectex buffer data from a
// parsed object
std::vector<AggVertex_pos_tex> aggregatePosTex(Object3D &obj);

#endif
