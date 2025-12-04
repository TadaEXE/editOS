#pragma once
#include <cstdarg>

#include "hal/serial.hpp"
#include "kernel/logging/logging.hpp"

namespace logging::backend {

class SerialSink final : public LoggingSink {
 public:
  explicit SerialSink(hal::SerialBus& serial) : serial(serial) {}

  void put_char(char c) noexcept override { serial.write_char(c); }

 private:
  hal::SerialBus& serial;
};

}  // namespace logging::backend
