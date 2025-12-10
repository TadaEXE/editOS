#pragma once
#include <cstdarg>

#include "drv/serial/serial.hpp"
#include "hal/serial.hpp"
#include "logging/logging.hpp"

namespace logging::backend {

class SerialSink final : public LoggingSink {
 public:
  SerialSink(const SerialSink&) = delete;
  SerialSink(SerialSink&&) = delete;
  SerialSink& operator=(const SerialSink&) = delete;
  SerialSink& operator=(SerialSink&&) = delete;

  explicit SerialSink(drv::serial::Port& serial) : serial(serial) {}

  void put_char(char c) const noexcept override {
    serial.write_byte(static_cast<uint8_t>(c));
  }

 private:
  drv::serial::Port& serial;
};

}  // namespace logging::backend
