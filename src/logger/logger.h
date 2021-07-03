#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>
#include <string>

enum class LoggerState : char {
  Info = 0, Debug = 1, Error = 2,
};

static const char *statePrefix[3] = {
  "[INFO]  ", "[DEBUG] ", "[ERROR] ",
};

class Logger {
  static const bool m_loggingEnabled =
#ifdef __NO_LOG__
  false;
#else
  true;
#endif
  LoggerState m_state;

public:
  Logger();
  ~Logger() = default;

  // reset the state of the logger to LoggerState::Info
  inline void reset() { m_state = LoggerState::Info; }

  // set the state of the logger (sticky)
  Logger &operator<<(LoggerState state);

  template<typename T>
  Logger &operator<<(const T &v) {
    if (!m_loggingEnabled) return *this;
    std::ostream &os = (m_state == LoggerState::Error) ? std::cout : std::cerr;
    os << v;
    return *this;
  }
};


#endif
