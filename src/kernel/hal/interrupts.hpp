#pragma once

#include <cstdint>

namespace hal {

struct Interrupt {
  uint8_t vector;
};

class InterruptController {
 public:
  using HandlerFn = void (*)(void*);

  virtual ~InterruptController() = default;

  virtual void register_irq(Interrupt line, HandlerFn handler, void* ctx) noexcept = 0;
  virtual void enable_irq(Interrupt line) noexcept = 0;
  virtual void disable_irq(Interrupt line) noexcept = 0;
  virtual void send_eoi(Interrupt line) noexcept = 0;
};

}  // namespace hal
