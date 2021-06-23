#ifndef __STATUS_H__
#define __STATUS_H__

#include <string>

// StatusType is used to categorize a Status.
enum StatusType {
  Info = 0, Debug = 1, Error = 2,
};

// Status prefixes for the log messages.
static const char *StatusTypeToString[3] = {
  "[INFO]", "[DEBUG]", "[ERROR]"
};

const std::string EmptyStatus = "";

// Status is a generic object which represents the outcome of an operation.
// It contains functionality for logging status messages to std::cerr.
class Status {
  StatusType  m_type;
  std::string m_logMsg;

public:
  Status();
  Status(StatusType type, const std::string msg);
  ~Status() = default;

  StatusType getStatus();
  std::string &getMsg();

  void setStatus(StatusType type);
  void setMsg(const std::string msg);

  // isError returns true if the internal status is set to StatusType::Error and
  // false otherwise.
  bool isError();
  // log logs the status to std::cerr, indicating the status type as well.
  // A newline is inserted after the logMsg is outputted, so the msg need not
  // have newline at the end.
  // If `errFatal` is set to true and the internal status type is
  // StatusType::Error, exit(1) is called after logging the status.
  void log(bool errFatal);
  // log logs the status to std::cerr.
  // log is an overload for which `errFatal` is set to `false` by default.
  void log();
  // clear resets the internal state.
  void clear();
};

#endif
