#pragma once

#include "boot/boot_context.hpp"
#include "drv/serial/serial.hpp"
#include "hal/framebuffer.hpp"
#include "hal/interrupts.hpp"
#include "hal/keyboard.hpp"
#include "hal/paging.hpp"
#include "hal/timer.hpp"

namespace kernel {

struct KernelServices {
  hal::Framebuffer* framebuffer;
  drv::serial::Port* serial;

  hal::Keyboard* keyboard;
  hal::Timer* timer;
  hal::InterruptController* interrupt_controller;
  hal::Paging* paging;
};

class Kernel {
 public:
  Kernel(KernelServices& services, boot::BootContext& ctx)
      : services(services), ctx(ctx) {}

  [[noreturn]] void enter();

 private:
  KernelServices& services;
  boot::BootContext& ctx;
};

}  // namespace kernel
