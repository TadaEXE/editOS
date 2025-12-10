#pragma once

#include <cstdint>

namespace board {

struct SerialDesc {
  const char* name;
  uintptr_t io_base;
  uint8_t irq;
};

}  // namespace board
