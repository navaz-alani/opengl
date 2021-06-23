#include <fstream>
#include <sstream>
#include <cstdlib>

#include "loader.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

const char *shaderName(const unsigned int shaderType) {
  switch (shaderType) {
    case GL_VERTEX_SHADER:   return "Vertex Shader";
    case GL_FRAGMENT_SHADER: return "Fragment Shader";
    default:                 return "Geometry Shader";
  }
}

void compileShader(
  unsigned int shaderType,
  const std::string &filename,
  LoaderStatus *status
) {
  unsigned int shaderID = glCreateShader(shaderType);

  std::ifstream sourceFile{ filename };
  // confirm that source file has been opened
  if (!sourceFile.is_open()) {
    status->setStatus(StatusType::Error);
    status->setMsg(std::string("failed to open file ") + filename);
    status->assetID = 0;
    return;
  }
  // read source code into buffer
  std::stringstream buffer;
  buffer << sourceFile.rdbuf();
  const std::string sourceCode = buffer.str();
  const char *shaderSource = sourceCode.c_str();
  // setup shader and compile
  glShaderSource(shaderID, 1, &shaderSource, nullptr);
  glCompileShader(shaderID);
  // check if compilation succeeded
  int compileSuccess;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileSuccess);
  if (compileSuccess == GL_FALSE) {
    // compilation failed - get error message
    int errMsgLen;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &errMsgLen);
    std::string errMsg(errMsgLen+1, '\0');
    glGetShaderInfoLog(shaderID, errMsgLen, &errMsgLen, (char *)((void *)errMsg.c_str()));
    // cleanup and set status for caller
    glDeleteShader(shaderID);

    status->setStatus(StatusType::Error);
    status->setMsg(errMsg);
    status->assetID = 0;

    return;
  }
  status->setStatus(StatusType::Debug);
  status->setMsg(std::string("compiled ") + shaderName(shaderType) + " : " + filename);
  status->assetID = shaderID;
}

ShaderLoader::ShaderLoader(const ShaderSource &src)
  : m_vertexShaderFile{ src.vertexShaderFile }, m_fragShaderFile{ src.fragmentShaderFile }
{}

ShaderLoader::ShaderLoader(
  const std::string vertexShaderFile,
  const std::string fragShaderFile
) : m_vertexShaderFile{ vertexShaderFile }, m_fragShaderFile{ fragShaderFile }
{}

LoaderStatus ShaderLoader::CreateProgram() {
  LoaderStatus status;
  unsigned int programID = glCreateProgram();
  unsigned int vertexShaderID{ 0 }, fragmentShaderID{ 0 };

  // compile and attach vertex shader
  compileShader(GL_VERTEX_SHADER, m_vertexShaderFile, &status);
  if (status.isError()) goto compile_error;
  vertexShaderID = status.assetID;
  glAttachShader(programID, vertexShaderID);
  status.log(); status.clear();
  // compile and attach fragment shader
  compileShader(GL_FRAGMENT_SHADER, m_fragShaderFile, &status);
  if (status.isError()) goto compile_error;
  fragmentShaderID = status.assetID;
  glAttachShader(programID, fragmentShaderID);
  status.log(); status.clear();

  status.setStatus(StatusType::Debug);
  status.setMsg("successfully created shader program");
  status.assetID = programID;

  glLinkProgram(programID);
  glValidateProgram(programID);

  glDeleteShader(vertexShaderID);
  glDeleteShader(fragmentShaderID);

  return status;

compile_error:
  glDeleteShader(vertexShaderID);
  glDeleteShader(fragmentShaderID);

  glDeleteProgram(programID);

  return status;
}
