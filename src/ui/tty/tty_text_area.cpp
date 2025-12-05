#include "ui/tty/tty_text_area.hpp"

namespace ui {

void TtyTextArea::move_left(size_t amount) noexcept {
  auto pos = area.cursor_pos();
  pos.x = amount >= pos.x ? pos.x : pos.x - amount;
  area.move_cursor(pos);
}

void TtyTextArea::move_right(size_t amount) noexcept {
  auto pos = area.cursor_pos();
  pos.x += amount;
  area.move_cursor(pos);
}

void TtyTextArea::move_up(size_t amount) noexcept {
  auto pos = area.cursor_pos();
  pos.y = amount > pos.y ? pos.y : pos.y - amount;
  area.move_cursor(pos);
}

void TtyTextArea::move_down(size_t amount) noexcept {
  auto pos = area.cursor_pos();
  pos.y += amount;
  area.move_cursor(pos);
}

void TtyTextArea::move_end() noexcept {
  auto pos = area.cursor_pos();
  pos.x = area.current_line_length();
  area.move_cursor(pos);
}
}  // namespace ui
