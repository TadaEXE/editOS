#include <source_location>

#include <kernel/log.hpp>
#include <kernel/panic.hpp>

#include "hal/system.hpp"

namespace internal {
[[noreturn]] void panic_impl(const std::source_location location, const char* fmt, ...) {

  log_msg("\n[KERNEL PANIC] \\");
  va_list args;
  va_start(args, fmt);
  log_msg(fmt, args);
  va_end(args);
  log_msg("at %s:%d in `%s`", location.file_name(), location.line(),
          location.function_name());

  hal::sys::halt();
}
}  // namespace internal
