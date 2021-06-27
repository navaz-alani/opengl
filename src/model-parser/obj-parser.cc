#include "obj-parser.h"

#include <fstream>

ObjParser::ObjParser()
  : m_parseError{ false }, m_errorMsg{ "" }
{}

Object3D ObjParser::parse(const std::string &objFile) {
  std::ifstream objf{ objFile };
  if (!objf.is_open()) {
    m_parseError = true;
    m_errorMsg = "failed to open object file: " + objFile;
  }

  Object3D obj;

  unsigned char idx = 0;
  char declType; float posCoord; unsigned int vertexIdx;
  position_3d_t vertex{0};
  face_t face{0};
  while (objf.good()) {
    if (idx == 0) {
      // read in the type of the declaration
      objf >> declType;
      if (!(declType == 'v' || declType == 'f')) {
        m_parseError = true;
        m_errorMsg = std::string("unknown declaration type ") + declType;
        return emptyObj;
      }
    } else {
      // read in the appropriate value and update the loop parse variable
      // (vertex or face) being parsed
      if (declType == 'v') {
        objf >> posCoord;
        if (idx == 1)      vertex.x = posCoord;
        else if (idx == 2) vertex.y = posCoord;
        else               vertex.z = posCoord;
      } else {
        objf >> vertexIdx;
        // since the obj files refer to vertices as 1-indexed, we have to adjust
        --vertexIdx;
        if (idx == 1)      face.v0 = vertexIdx;
        else if (idx == 2) face.v1 = vertexIdx;
        else               face.v2 = vertexIdx;
      }
      // check if we have finished parsing the line
      if (idx == 3) {
        // we add to the object and reset the loop parse variable
        if (declType == 'v') {
          obj.vertexPositions.emplace_back(vertex);
          vertex = {0}; 
        } else {
          obj.faces.emplace_back(face);
          face = {0};
        }
      }
    }
    idx = (++idx) % 4;
  }

  /*
  for (unsigned int i = 0; i < obj.vertexPositions.size(); ++i) {
    auto v = obj.vertexPositions[i];
    std::cout << "{"
              << v.x << " "
              << v.y << " "
              << v.z << "}\n";
  }
  for (unsigned int i = 0; i < obj.faces.size(); ++i) {
    auto v = obj.faces[i];
    std::cout << "{"
              << v.v0 << " "
              << v.v1 << " "
              << v.v2 << "}\n";
  }
  */

  return obj;
}
