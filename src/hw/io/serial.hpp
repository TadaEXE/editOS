#pragma once
#include "hw/io/ports.hpp"

#include <cstdint>

namespace hw::io {

constexpr uint16_t COM1_BASE = 0x3F8;

class SerialPort {
public:
  SerialPort(uint16_t com_base)
      : com_base(com_base), data(com_base + 0), int_en(com_base + 1),
        fifo_ctrl(com_base + 2), line_ctrl(com_base + 3),
        modem_ctrl(com_base + 4), line_status(com_base + 5) {}

  void init() noexcept;
  void write_char(char c) noexcept;
  void write_string(const char *s) noexcept;
  void write_hex32(uint32_t v) noexcept;

private:
  void write_raw(char c) noexcept;

  const uint16_t com_base;

  Port8 data;
  Port8 int_en;
  Port8 fifo_ctrl;
  Port8 line_ctrl;
  Port8 modem_ctrl;
  Port8 line_status;
};
} // namespace hw::io
