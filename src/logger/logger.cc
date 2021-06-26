#include "logger.h"

Logger::Logger()
  : m_state{ LoggerState::Info }
{}

Logger &Logger::operator<<(LoggerState state) {
  m_state = state;
  return *this;
}

void Logger::operator<<(const std::string &msg) {
  std::ostream &os = (m_state == LoggerState::Error) ? std::cout : std::cerr;
  os << statePrefix[static_cast<int>(m_state)] << msg << "\n";
}
