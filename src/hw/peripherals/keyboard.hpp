#pragma once

#include "hw/io/ports.hpp"

namespace hw::peripheral {

class Keyboard {
public:
  uint8_t read_scancode() const noexcept;

private:
  io::Port8 data_{0x60};
  io::Port8 status_{0x64};
};

} // namespace hw::peripheral
