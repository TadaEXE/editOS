#pragma once

#include <source_location>
#include <utility>

namespace internal {
[[noreturn]] void panic_impl(const std::source_location location, const char* fmt, ...);
}

template <typename... Args>
struct panic {
  [[noreturn]] panic(
      const char* fmt, Args&&... args,
      const std::source_location location = std::source_location::current()) {
    internal::panic_impl(location, fmt, std::forward<Args>(args)...);
  }
};

template <typename... Args>
panic(const char*, Args&&...) -> panic<Args...>;
