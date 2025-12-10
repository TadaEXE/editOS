#include "x86/common/drv/serial_16550/serial_16550.hpp"

namespace x86::drv {

void Serial16550Impl::init() noexcept {
  // Disable interrupts
  int_en.out(0x00);

  // Enable DLAB (set baud divisor)
  line_ctrl.out(0x80);

  // Set divisor for 38400 baud: divisor = 3
  data.out(0x03);    // low byte
  int_en.out(0x00);  // high byte

  // 8 bits, no parity, one stop bit, clear DLAB
  line_ctrl.out(0x03);

  // Enable FIFO, clear them, 14-byte threshold
  fifo_ctrl.out(0xC7);

  // IRQs enabled, RTS/DSR set
  modem_ctrl.out(0x0B);

  inited = true;
}

bool Serial16550Impl::write_byte(uint8_t b) noexcept {
  if (!inited) return false;

  if (b == '\n') {
    write_raw('\r');
    write_raw('\n');
  } else {
    write_raw(b);
  }

  return true;
}

bool Serial16550Impl::read_byte(uint8_t& bout) noexcept {
  if (!inited) return false;
  if ((line_status.in() & 0x01) == 0) return false;

  bout = data.in();
  return true;
}

bool Serial16550Impl::write(const uint8_t* data, size_t len) noexcept {
  if (!data) return false;
  if (!inited) return false;

  for (size_t i = 0; i < len; ++i) {
    write_raw(data[i]);
  }

  return true;
}

void Serial16550Impl::write_raw(uint8_t b) const noexcept {
  while ((line_status.in() & 0x20) == 0) {}
  data.out(b);
}

}  // namespace x86::drv
