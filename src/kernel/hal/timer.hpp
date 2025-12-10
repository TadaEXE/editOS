#pragma once

#include <cstdint>

namespace hal {

class Timer {
 public:
  virtual ~Timer() = default;

  virtual void init(uint32_t frequnecy_hz) noexcept = 0;
  virtual uint64_t ticks() const noexcept = 0;
  virtual void sleep_ms(uint32_t ms) noexcept = 0;
};

}  // namespace hal
