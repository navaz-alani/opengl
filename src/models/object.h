#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "../buffer/layout.h"
#include "material-parser.h"

// a 3D coordinate position
struct position_3d_t {
  float x, y, z;

  inline position_3d_t& operator+=(const position_3d_t& rhs) {
    x += rhs.x; y += rhs.y; z += rhs.z;
    return *this;
  }

  inline position_3d_t operator-(const position_3d_t &rhs) {
    return { x - rhs.x, y - rhs.y, z - rhs.z };
  }

  inline position_3d_t operator*(const position_3d_t &rhs) {
    position_3d_t normal;
    normal.x = y*rhs.z - z*rhs.y;
    normal.y = z*rhs.x - x*rhs.z;
    normal.z = x*rhs.y - y*rhs.x;
    return normal;
  }

  inline void normalize() {
    float norm = sqrt(x*x + y*y + z*z);
    x /= norm; y /= norm; z /= norm;
  }
};

// a normal vector
typedef position_3d_t v_normal_t;
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
  std::vector<Material>      materialPalette;
  std::string                material;

  void computeNormals();
};

/*
 * VERTEX AGGREGATION
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

// aggregate vertex position and texture coordinate vertex buffer data from a
// parsed object
std::vector<AggVertex_pos_tex> aggregatePosTex(Object3D &obj);

// vertex with two attributes - position and texture coordinates
struct AggVertex_pos_tex_nor {
public:
  position_3d_t m_pos;
  tex_2d_t      m_tex;
  v_normal_t    m_nor;

  AggVertex_pos_tex_nor(position_3d_t pos, tex_2d_t tex, v_normal_t nor);
  ~AggVertex_pos_tex_nor() = default;

  BufferLayout getLayout() const;
};

// aggregate vertex position, texture coordinate and normal vector vertex buffer
// data from a parsed object
std::vector<AggVertex_pos_tex_nor> aggregatePosTexNor(Object3D &obj);

#endif
