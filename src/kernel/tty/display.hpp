#pragma once

#include <cstddef>

namespace tty {

class Display {
 public:
  virtual ~Display() = default;
  virtual void put_char(char c) noexcept = 0;
  virtual void backspace() noexcept = 0;
  virtual void flush() noexcept = 0;
  virtual void scroll_up(size_t s) noexcept = 0;
  virtual void scroll_down(size_t s) noexcept = 0;
};
}  // namespace tty
