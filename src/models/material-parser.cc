#include "material-parser.h"
#include "../logger/logger.h"

#include <fstream>
#include <limits>

/*
 * LINE PARSERS
 */

inline rgb_color_t parseColor(std::istream &in) {
  rgb_color_t color;
  in >> color.r; in >> color.g; in >> color.b;
  return color;
}

inline float parseFloat(std::istream &in) {
  float f;
  in >> f;
  return f;
}

inline std::string parseString(std::istream &in) {
  std::string str;
  in >> str;
  return str;
}

inline void parseIgnoreLine(std::istream &in) {
  in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/*
 * MATERIAL FILE PARSER
 */

std::vector<Material> MtlParser::parse(const std::string &matFile) {
  std::ifstream matf{ matFile };
  if (!matf.is_open()) {
    m_parseError = true;
    m_errorMsg = "failed to open material file: " + matFile;
  }

  Logger log;
  std::vector<Material> materials;
  bool mtlDeclared{ false };
  Material mat;
  std::string declType{ DECL_INVALID };

  while (matf.good()) {
    matf >> declType;
    if (matf.eof()) break;
    if (declType == DECL_COMMENT)
      parseIgnoreLine(matf);
    else if (declType == MTL_DECL_MTL) {
      // if we have a previously declared material, we add the previously parsed
      // one to our materials list
      if (!mtlDeclared) mtlDeclared = true;
      else { materials.push_back(mat); mat = {}; }
      mat.name = parseString(matf);
    } else if (declType == MTL_DECL_AMBIENT_COLOR)
      mat.ambientColor = parseColor(matf);
    else if (declType == MTL_DECL_DIFFUSE_COLOR)
      mat.diffuseColor = parseColor(matf);
    else if (declType == MTL_DECL_SPECULAR_COLOR)
      mat.specularColor = parseColor(matf);
    else if (declType == MTL_DECL_TRANSMISSION_FILTER)
      mat.transmissionFilterColor = parseColor(matf);
    else if (declType == MTL_DECL_OPTICAL_DENSITY)
      mat.opticalDensity = parseFloat(matf);
    else if (declType == MTL_DECL_TRANSPARENCY)
      mat.transparency = parseFloat(matf);
    else if (declType == MTL_DECL_SPECULAR_EXPONENT)
      mat.specularExponent = parseFloat(matf);
    // handle the sunsupported and ignored declarations
    else if (declType == MTL_DECL_ILLUMINATION         ||
             declType == MTL_DECL_ILLUMINATION         ||
             declType == MTL_DECL_DIFFUSE_TX_MAP       ||
             declType == MTL_DECL_SPECULAR_TX_MAP      ||
             declType == MTL_DECL_SPECULAR_HL_MAP      ||
             declType == MTL_DECL_ALPHA_TX_MAP         ||
             declType == MTL_DECL_BUMP_MAP             ||
             declType == MTL_DECL_DISPLACEMENT_MAP     ||
             declType == MTL_DECL_EMISSIVITY           ||
             declType == MTL_DECL_STENCIL_DECAL_TX_MAP) {
      parseIgnoreLine(matf);
      log << LoggerState::Debug
          << "mtl parser: ignoring unsupported decl "
          << declType
          << "\n";
    } else {
      m_parseError = true;
      m_errorMsg = std::string("unknown declaration type ") + declType;
      return {};
    }
    declType = DECL_INVALID;
  }
  materials.push_back(mat);
  return materials;
}
