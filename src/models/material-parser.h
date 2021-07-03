#ifndef __MATERIAL_PARSER_H__
#define __MATERIAL_PARSER_H__

#include <string>
#include <vector>

#include "parser.h"
#include "material.h"

// these are the supported declarations in the material file

static const char *MTL_DECL_MTL                 = "newmtl";
static const char *MTL_DECL_AMBIENT_COLOR       = "Ka";
static const char *MTL_DECL_DIFFUSE_COLOR       = "Kd";
static const char *MTL_DECL_OPTICAL_DENSITY     = "Ni";
static const char *MTL_DECL_SPECULAR_COLOR      = "Ks";
static const char *MTL_DECL_SPECULAR_EXPONENT   = "Ns";
static const char *MTL_DECL_TRANSPARENCY        = "d";
static const char *MTL_DECL_TRANSMISSION_FILTER = "Tf";
static const char *MTL_DECL_AMBIENT_TX_MAP      = "map_Ka";

// these are the recognized, but unsupported and hence ignored, declarations

static const char *MTL_DECL_ILLUMINATION         = "illum";
static const char *MTL_DECL_EMISSIVITY           = "Ke";
static const char *MTL_DECL_DIFFUSE_TX_MAP       = "map_Kd";
static const char *MTL_DECL_SPECULAR_TX_MAP      = "map_Ks";
static const char *MTL_DECL_SPECULAR_HL_MAP      = "map_Ns";
static const char *MTL_DECL_ALPHA_TX_MAP         = "map_d";
static const char *MTL_DECL_BUMP_MAP             = "map_bump";
static const char *MTL_DECL_DISPLACEMENT_MAP     = "disp";
static const char *MTL_DECL_STENCIL_DECAL_TX_MAP = "decal";

class MtlParser : public Parser {
public:
  MtlParser() : Parser() {};
  ~MtlParser() = default;

  // `parse` takes in a material file, parses it and returns the parsed Material
  // objects. The caller ought to check for any parsing errors using the
  // `isParseError` method and `getParseError` to determine the error if any.
  std::vector<Material> parse(const std::string &matFile);
};

#endif
