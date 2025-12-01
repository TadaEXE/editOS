#pragma once

#include <utility>

#include "kernel/log.hpp"

template <typename... Args>
[[noreturn]] void panic(const char* fmt, Args&&... args) {
  log_msg("[KERNEL PANIC] \\");
  log_msg(fmt, std::forward<Args>(args)...);
  for (;;) {}
}
