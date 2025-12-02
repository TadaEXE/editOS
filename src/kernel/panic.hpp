#pragma once

#include <source_location>
#include <utility>

#include "kernel/log.hpp"

template <typename... Args>
[[noreturn]] void panic(
    const char* fmt, Args&&... args,
    const std::source_location location = std::source_location::current()) {
  log_msg("[KERNEL PANIC] \\");
  log_msg(fmt, std::forward<Args>(args)...);
  log_msg("at %s:%d in `%s`", location.file_name(), location.line(),
          location.function_name());
  for (;;) {}
}
