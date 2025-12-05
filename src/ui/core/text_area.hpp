#pragma once

#include "gfx/text/text.hpp"
#include "kernel/containers/gap_buffer.hpp"

namespace ui {
class TextArea {
 public:
  TextArea(const TextArea&) = delete;
  TextArea(TextArea&&) = delete;
  TextArea& operator=(const TextArea&) = delete;
  TextArea& operator=(TextArea&&) = delete;

  TextArea(gfx::Rect area, gfx::Canvas& canvas, gfx::text::Style style)
      : area(area), canvas(canvas), tr(canvas, style), style(style) {}

  void put_char(char c);
  void put_text(const char* text);

  void remove_last();
  void remove_next();

  void redraw();

  void move_cursor(gfx::Point np);

  void scroll_up(size_t amount = 1);
  void scroll_down(size_t amount = 1);

  gfx::Point cursor_pos() const { return real_cursor; }

  size_t current_line_length() const noexcept;

 private:
  size_t visible_line_capacity() const noexcept;
  size_t count_lines() noexcept;
  size_t line_start_index(size_t line) const noexcept;
  size_t line_height() const noexcept;

  gfx::Rect area;
  gfx::Canvas& canvas;
  gfx::text::TextRenderer tr;
  gfx::text::Style style;

  gfx::Point real_cursor{0, 0};
  gfx::Point target_cursor{0, 0};

  ctr::GapBuffer<char, 16> buffer{};
  size_t real_cursor_idx{0};

  size_t lines{0};
  size_t first_visible_line{0};

  bool follow_bottom{true};
};
}  // namespace ui
