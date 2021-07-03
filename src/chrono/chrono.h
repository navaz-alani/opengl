#ifndef __CHRONO_H__
#define __CHRONO_H__

#include <chrono>
#include <string>
#include <iostream>
#include <iomanip>

#include "../logger/logger.h"

using namespace std::chrono;

// Chrono is a timing object which contains an internal data point containing
// the previous time measurement.
// Upon construction, the time of construction is set to the previous time
// measurement.
class Chrono {
  high_resolution_clock::time_point m_previous;

public:
  Chrono() : m_previous{ high_resolution_clock::now() } {}
  ~Chrono() = default;

  // measure sets the previous time measurement to the current time.
  inline void measure() {
    m_previous = high_resolution_clock::now();
  }
  // log logs the time encountered since the previous measurement, with the
  // specified message.
  inline void log(const std::string &msg) {
    auto currTime = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(currTime - m_previous);
    m_previous = currTime;
    Logger log;
    log << LoggerState::Info
        << std::setw(10)
        << std::to_string(duration.count())
        << " microseconds | "
        << msg << "\n";
  }
};

#endif
