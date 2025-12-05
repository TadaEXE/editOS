#pragma once

#include "kernel/tty/display.hpp"
#include "ui/core/text_area.hpp"

namespace ui {

class TtyTextArea : public tty::Display {
 public:
  TtyTextArea(const TtyTextArea&) = delete;
  TtyTextArea(TtyTextArea&&) = delete;
  TtyTextArea& operator=(const TtyTextArea&) = delete;
  TtyTextArea& operator=(TtyTextArea&&) = delete;

  explicit TtyTextArea(TextArea& area) noexcept : area(area) {}

  void put_char(char c) noexcept override { area.put_char(c); }

  void backspace() noexcept override { area.remove_last(); }

  void flush() noexcept override { area.redraw(); }

 private:
  TextArea& area;
};

}  // namespace ui
