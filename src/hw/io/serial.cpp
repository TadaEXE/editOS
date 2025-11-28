#include "hw/io/serial.hpp"

namespace hw::io {

void SerialPort::init() noexcept {
  // Disable interrupts
  int_en.out(0x00);

  // Enable DLAB (set baud divisor)
  line_ctrl.out(0x80);

  // Set divisor for 38400 baud: divisor = 3
  data.out(0x03);   // low byte
  int_en.out(0x00); // high byte

  // 8 bits, no parity, one stop bit, clear DLAB
  line_ctrl.out(0x03);

  // Enable FIFO, clear them, 14-byte threshold
  fifo_ctrl.out(0xC7);

  // IRQs enabled, RTS/DSR set
  modem_ctrl.out(0x0B);
}

void SerialPort::write_char(char c) noexcept {
  if (c == '\n') {
    write_raw('\r');
    write_raw('\n');
  } else {
    write_raw(c);
  }
}

void SerialPort::write_string(const char *s) noexcept {
  while (*s) {
    write_char(*s++);
  }
}

void SerialPort::write_hex32(uint32_t v) noexcept {
  static const char *hex = "0123456789ABCDEF";
  write_string("0x");
  for (int8_t i = 7; i >= 0; --i) {
    uint8_t nibble = (v >> (i * 4)) & 0xF;
    write_char(hex[nibble]);
  }
}

void SerialPort::write_raw(char c) noexcept {

  while ((line_status.in() & 0x20) == 0) {
  }
  data.out(static_cast<uint8_t>(c));
}
} // namespace hw::io
