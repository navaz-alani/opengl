#ifndef __OBJ_PARSER_H__
#define __OBJ_PARSER_H__

#include <iostream>
#include <string>
#include <vector>

#include "object.h"

static const char *DECL_INVALID  = "_";
static const char *DECL_COMMENT  = "#";
static const char *DECL_VERTEX   = "v";
static const char *DECL_FACE     = "f";
static const char *DECL_UV_COORD = "vt";
static const char *DECL_V_NORM   = "vn";

const Object3D emptyObj;

// ObjParser parses object model files which have the following strict format
// for each line in the file:
//
// Lines with the following format declare a vertex position.
//
// v <x_coord:float> <y_coord:float> <z_coord:float>
//
// Lines with the following format declare a triangle using the line numbers of
// vertex position declarations.
//
// f <vertex_no:uint> <vertex_no:uint> <vertex_no:uint>
//
// Lines with the following formal declare a normal vector.
//
// n <x_coord:float> <y_coord:float> <z_coord:float>
//
// Lines with the following format declare a UV/texture coordinate
//
// vt <x_coord:float> <y_coord:float>
//
// If a line begins with a single '#' character, the entire line is considered
// to be a comment and therefore ignored while parsing.
class ObjParser {
  bool        m_parseError;
  std::string m_errorMsg;
public:
  ObjParser();
  ~ObjParser() = default;

  // `parse` takes in an object model file, parses it and returns the parsed 3D
  // object. The caller ought to check for any parsing errors using the
  // `isParseError` method and `getParseError` to determine the error if any.
  Object3D parse(const std::string &objFile);

  inline bool isParseError() const { return m_parseError; }
  inline const std::string &getParseError() const { return m_errorMsg; }
  inline void clearErrors() { m_parseError = false; m_errorMsg = ""; }
};

#endif
