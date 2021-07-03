#ifndef __PARSER_H__
#define __PARSER_H__

static const char *DECL_INVALID  = "_";
static const char *DECL_COMMENT  = "#";

class Parser {
protected:
  bool        m_parseError;
  std::string m_errorMsg;

public:
  Parser()
    : m_parseError{ false }, m_errorMsg{ "" }
  {}
  ~Parser() = default;

  inline bool isParseError() const { return m_parseError; }
  inline const std::string &getParseError() const { return m_errorMsg; }
  inline void clearErrors() { m_parseError = false; m_errorMsg = ""; }
};

#endif
