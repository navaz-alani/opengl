#ifndef __OBJ_PARSER_H__
#define __OBJ_PARSER_H__

#include <iostream>
#include <string>
#include <vector>

#include "parser.h"
#include "object.h"
#include "material-parser.h"

// supported object file declarations

static const char *OBJ_DECL_VERTEX   = "v";
static const char *OBJ_DECL_FACE     = "f";
static const char *OBJ_DECL_UV_COORD = "vt";
static const char *OBJ_DECL_V_NORM   = "vn";
static const char *OBJ_DECL_MTL_EXT  = "mtllib";
static const char *OBJ_DECL_MTL_USE  = "usemtl";

// unsupported/ignored object file declarations

static const char *OBJ_DECL_OBJ_NAME       = "o";
static const char *OBJ_DECL_GRP_NAME       = "g";
static const char *OBJ_DECL_SMOOTH_SHADING = "s";

const Object3D emptyObj;

// ObjParser reads in object files according to the Wavefront specification and
// returns the parsed Object3D.
class ObjParser : public Parser {
public:
  ObjParser() : Parser() {};
  ~ObjParser() = default;

  // `parse` takes in an object model file, parses it and returns the parsed 3D
  // object. The caller ought to check for any parsing errors using the
  // `isParseError` method and `getParseError` to determine the error if any.
  Object3D parse(const std::string &objFile);
};

#endif
