#include "hw/peripherals/keyboard.hpp"

namespace hw::peripheral {

uint8_t Keyboard::read_scancode() const noexcept {
  while ((status_.in() & 0x01u) == 0u) {
    // spin
  }
  return data_.in();
}

} // namespace hw::peripheral
