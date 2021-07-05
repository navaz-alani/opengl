#include "obj-parser.h"
#include "../logger/logger.h"

#include <fstream>
#include <sstream>
#include <limits>

// pathJoinDirFile joins the given directory path to the given file name using
// the OS specific path separator.
inline std::string pathJoinDirFile(std::string dir, std::string file) {
  char pathSeparator = '/';
#if defined(_WIN32)
  pathSeparator = '\\';
#endif
  return dir + pathSeparator + file;
}

// pathGetFileDir returns the directory that the given file is in.
inline std::string pathGetFileDir(std::string file) {
  return file.substr(0, file.find_last_of("/\\"));
}

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
  std::vector<unsigned int> tmpVertexIndices,
                            tmpTexIndices,
                            tmpNormalIndices;
  int numIndices = 0;
  while (true) {
    std::string facePart; in >> facePart;
    if (facePart.size() == 0) break;
    // a well-formed face part (vertex/texture/normal) MUST start with a digit
    if ('0' > facePart[0] || facePart[0] > '9') {
      for (char c : facePart) in.putback(c);
      break;
    }
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
    ++numIndices;
    tmpVertexIndices.push_back(vIdx);
    tmpTexIndices.   push_back(uvIdx);
    tmpNormalIndices.push_back(nIdx);
  }
  // convert an arbitrary n-sided polygon into a set of triangles
  assert(numIndices >= 3);
  // assumption - we must have vertex selections in a face
  // if texture and normal indices are not specified in the first specification,
  // we assume that they are not specified throughout the whole file.
  bool usingTex    { tmpTexIndices[0]    != 0 },
       usingNormals{ tmpNormalIndices[0] != 0 };
  for (unsigned int offset = 1; offset < numIndices-1; ++offset) {
    vertexIndices.push_back(tmpVertexIndices[0]-1);       // add vertex indices
    vertexIndices.push_back(tmpVertexIndices[offset]-1);
    vertexIndices.push_back(tmpVertexIndices[offset+1]-1);
    if (usingTex) {
      texIndices.push_back(tmpTexIndices[0]-1);           // add texture indices
      texIndices.push_back(tmpTexIndices[offset]-1);
      texIndices.push_back(tmpTexIndices[offset+1]-1);
    }
    if (usingNormals) {
      normalIndices.push_back(tmpNormalIndices[0]-1);     // add normal indices
      normalIndices.push_back(tmpNormalIndices[offset]-1);
      normalIndices.push_back(tmpNormalIndices[offset+1]-1);
    }
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

inline void parseIgnoreLine(std::istream &in) {
  in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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

  Logger log;
  Object3D obj; obj.boundingBox = { 0 };
  std::string declType{ DECL_INVALID };
  while (objf.good()) {
    objf >> declType;
    if (objf.eof()) break;
    if (declType == DECL_COMMENT)
      parseIgnoreLine(objf);
    else if (declType == OBJ_DECL_VERTEX) {
      position_3d_t pos = parseVertex(objf);
      obj.boundingBox.update(pos);
      obj.vertexPositions.push_back(pos);
    } else if (declType == OBJ_DECL_FACE)
      parseFace(objf, obj.vertexIndices, obj.texIndices, obj.normalIndices);
    else if (declType == OBJ_DECL_UV_COORD)
      obj.texCoords.push_back(parseTexCoord(objf));
    else if (declType == OBJ_DECL_V_NORM)
      obj.normals.push_back(parseNormal(objf));
    else if (declType == OBJ_DECL_MTL_EXT) {
      std::string mtlFile; objf >> mtlFile;
      MtlParser mtlp;
      std::vector<Material> mtls = mtlp.parse(
          pathJoinDirFile(pathGetFileDir(objFile), mtlFile)
      );
      if (mtlp.isParseError()) {
        m_parseError = true;
        m_errorMsg = std::string("mtl parse error ") + mtlp.getParseError();
        return emptyObj;
      }
      for (auto & mtl : mtls) obj.materialPalette.push_back(mtl);
    } else if (declType == OBJ_DECL_MTL_USE) {
      objf >> obj.material;
      // check if the material exists - if not, we have an error
      bool materialDefined{ false };
      for (auto & mat : obj.materialPalette)
        if (mat.name == obj.material) { materialDefined = true; break; }
      if (!materialDefined) {
        m_parseError = true;
        m_errorMsg = std::string("obj parse error: undefined material ") + obj.material;
        return emptyObj;
      }
    } else if (declType == OBJ_DECL_OBJ_NAME ||
               declType == OBJ_DECL_GRP_NAME ||
               declType == OBJ_DECL_SMOOTH_SHADING) {
      parseIgnoreLine(objf);
      log << LoggerState::Debug
          << "obj parser: ignoring unsupported decl "
          << declType
          << "\n";
    } else {
      m_parseError = true;
      m_errorMsg = std::string("unknown declaration type ") + declType;
      return emptyObj;
    }
    declType = DECL_INVALID;
  }

  return obj;
}
