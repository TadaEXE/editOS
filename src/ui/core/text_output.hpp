#pragma once

#include "gfx/text/text.hpp"
#include "kernel/containers/gap_buffer.hpp"
#include "ui/core/window.hpp"

namespace ui {
class TextOutput : public Window {
 public:
  TextOutput(gfx::Rect rect, gfx::Canvas& canvas, gfx::text::Style style)
      : Window(rect), canvas(canvas), tr(canvas, style), style(style) {}

  void put_char(char c);
  void put_text(const char* text);
  void update();

  void move_cursor(gfx::Point np);

  void scroll_up(size_t s);
  void scroll_down(size_t s);

 private:
  gfx::Canvas& canvas;
  gfx::text::TextRenderer tr;
  gfx::text::Style style;
  gfx::Point cursor{0, 0};
  ctr::GapBuffer<char, 16> buffer;
};
}  // namespace ui
