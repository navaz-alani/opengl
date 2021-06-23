#include <iostream>

#include "status.h"

Status::Status()
  : m_type{ StatusType::Info }, m_logMsg{ EmptyStatus }
{}

Status::Status(StatusType type, const std::string msg)
  : m_type{ type }, m_logMsg{ msg }
{}

StatusType Status::getStatus() { return m_type; }
std::string &Status::getMsg() { return m_logMsg; }

void Status::setStatus(StatusType type) { m_type = type; }
void Status::setMsg(const std::string msg) { m_logMsg = msg; }

bool Status::isError() { return m_type == StatusType::Error; }

void Status::log(bool errFatal) {
  // we don't log if we don't have a valid message string
  if (!m_logMsg.size()) return;
  std::cerr << StatusTypeToString[m_type] << ": " << m_logMsg << '\n';
  if (m_type == StatusType::Error && errFatal) exit(1); // treat errors as fatal
}

void Status::log() { log(false); }

void Status::clear() {
  m_type = StatusType::Info;
  m_logMsg = EmptyStatus;
}
