#ifndef __OBJ_PARSER_H__
#define __OBJ_PARSER_H__

#include <iostream>
#include <string>
#include <vector>

// a 3D coordinate position
struct position_3d_t { float x, y, z; };
// vertex indices defining a 3D plane
struct face_t        { unsigned int v0, v1, v2; };

// A parsed 3D object.
struct Object3D {
  std::vector<position_3d_t> vertexPositions;
  std::vector<face_t>        faces;
};

const Object3D emptyObj;

// ObjParser parses object model files which have the following strict format
// for each line in the file:
//
// Lines with the following format declare a vertex position.
//
// v <float> <float> <float>
//
// Lines with the following format declare a triangle using the line numbers of
// vertex position declarations.
//
// f <line_no> <line_no> <line_no>
//
// An additional constraint is that all faces must be defined AFTER all vertex
// positions have been defined. This is to enable error checking.
//
// This object contains an internal error state for parsing operations.
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
