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
  LoggerState m_state;

public:
  Logger();
  ~Logger() = default;

  // reset the state of the logger to LoggerState::Info
  inline void reset() { m_state = LoggerState::Info; }

  // set the state of the logger (sticky)
  Logger &operator<<(LoggerState state);
  // log a *single* message to the logger, with the state prefix
  void operator<<(const std::string &msg);
};


#endif
