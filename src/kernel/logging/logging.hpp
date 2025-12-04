#pragma once

#include <concepts>
#include <utility>

namespace logging {
namespace backend {

class LoggingSink {
 public:
  virtual ~LoggingSink() = default;
  virtual void put_char(char c) noexcept = 0;
};

void set_sink(LoggingSink* sink);

void log_obj(const char* fmt, ...);
}  // namespace backend

class Loggable;

template <typename O>
concept LoggableObject = std::derived_from<O, Loggable> && requires {
  { O::fmt() } -> std::convertible_to<const char*>;
};

/// @note When implementing the `log_self` method, just call `log_obj` like this:
/// log_obj<T>(Args...); where the args are in the same order as given by the fmt
class Loggable {
 public:
  virtual ~Loggable() = default;

  virtual void log_self() const noexcept = 0;

 protected:
  template <LoggableObject O, typename... Args>
  void log_obj(Args&&... args) const noexcept {
    logging::backend::log_obj(O::fmt(), std::forward<Args>(args)...);
  }
};
}  // namespace logging
