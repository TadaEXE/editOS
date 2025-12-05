#pragma once


namespace tty {

class Display {
 public:
  virtual ~Display() = default;
  virtual void put_char(char c) noexcept = 0;
  virtual void backspace() noexcept = 0;
  virtual void flush() noexcept = 0;
};
}  // namespace tty
