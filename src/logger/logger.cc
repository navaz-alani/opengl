#include "logger.h"

Logger::Logger()
  : m_state{ LoggerState::Info }
{}

Logger &Logger::operator<<(LoggerState state) {
  if (!m_loggingEnabled) return *this;
  m_state = state;
  std::ostream &os = (m_state == LoggerState::Error) ? std::cout : std::cerr;
  os << statePrefix[static_cast<int>(m_state)];
  return *this;
}
