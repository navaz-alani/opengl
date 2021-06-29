#include "obj-parser.h"

#include <fstream>
#include <sstream>

ObjParser::ObjParser()
  : m_parseError{ false }, m_errorMsg{ "" }
{}

/*
 * LINE PARSERS
 */

position_3d_t parseVertex(std::istream &in) {
  position_3d_t vpos;
  in >> vpos.x; in >> vpos.y; in >> vpos.z;
  return vpos;
}

void parseFace(
  std::istream &in,
  std::vector<unsigned int> &vertexIndices,
  std::vector<unsigned int> &texIndices,
  std::vector<unsigned int> &normalIndices
) {
  for (unsigned char i = 0; i < 3; ++i) {
    std::string facePart; in >> facePart;
    unsigned int vIdx{ 0 }, uvIdx{ 0 }, nIdx{ 0 };
    char c;
    std::istringstream faceIn{ facePart };
    faceIn >> vIdx;
    if (faceIn.eof()) goto loop_end;
    faceIn >> c; // reads '/'
    faceIn >> c;
    if (c != '/') {
        faceIn.putback(c);
        faceIn >> uvIdx;
        if (faceIn.eof()) goto loop_end;
        faceIn >> c; // reads '/'
        faceIn >> nIdx;
    } else faceIn >> nIdx;
loop_end:
    if (vIdx != 0)  vertexIndices.push_back(--vIdx);
    if (uvIdx != 0) texIndices.   push_back(--uvIdx);
    if (nIdx != 0)  normalIndices.push_back(--nIdx);
  }
}

tex_2d_t parseTexCoord(std::istream &in) {
  tex_2d_t tc;
  in >> tc.x; in >> tc.y;
  return tc;
}

v_normal_t parseNormal(std::istream &in) {
  v_normal_t norm;
  in >> norm.x; in >> norm.y; in >> norm.z;
  return norm;
}

/*
 * OBJECT FILE PARSER
 */

Object3D ObjParser::parse(const std::string &objFile) {
  std::ifstream objf{ objFile };
  if (!objf.is_open()) {
    m_parseError = true;
    m_errorMsg = "failed to open object file: " + objFile;
  }

  Object3D obj;
  std::string declType{ DECL_INVALID };
  while (objf.good()) {
    objf >> declType;
    if (objf.eof()) break;
    if (declType == DECL_COMMENT) {
      objf.ignore('\n');
    } else if (declType == DECL_VERTEX) {
      obj.vertexPositions.push_back(parseVertex(objf));
    } else if (declType == DECL_FACE) {
      parseFace(objf, obj.vertexIndices, obj.texIndices, obj.normalIndices);
    } else if (declType == DECL_UV_COORD) {
      obj.texCoords.push_back(parseTexCoord(objf));
    } else if (declType == DECL_V_NORM) {
      obj.normals.push_back(parseNormal(objf));
    } else {
      m_parseError = true;
      m_errorMsg = std::string("unknown declaration type ") + declType;
      return emptyObj;
    }
    declType = DECL_INVALID;
  }

  /*
  for (auto v : obj.vertexPositions) {
    std::cout << "{"
              << v.x << " "
              << v.y << " "
              << v.z << "}\n";
  }
  for (auto v : obj.texCoords) {
    std::cout << "{"
              << v.x << " "
              << v.y << "}\n";
  }
  */

  return obj;
}
