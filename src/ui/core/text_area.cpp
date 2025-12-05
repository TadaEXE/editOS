#include "ui/core/text_area.hpp"

namespace ui {

void TextArea::put_char(char c) {
  buffer.insert(&c);
  ++target_cursor.x;
  ++real_cursor_idx;
  if (c == '\n' || c == '\r') {
    ++target_cursor.y;
    target_cursor.x = 0;
    ++lines;
  }
}

void TextArea::put_text(const char* text) {
  if (!text) return;

  for (const char* c = text; *c; c++) {
    put_char(*c);
  }
}

void TextArea::remove_last() {
  buffer.delete_left();
}

void TextArea::remove_next() {
  buffer.delete_left();
}

void TextArea::move_cursor(gfx::Point np) {
  target_cursor = np;
  if (target_cursor.y > lines) target_cursor.y = lines;
}

void TextArea::redraw() {
  canvas.clear(style.bg, area);
  gfx::Point draw_pos{0, 0};

  const auto cap = visible_line_capacity();
  if (cap == 0) return;

  const auto total = count_lines();
  if (total == 0) return;

  const auto bottom_start = total > cap ? total - cap : 0;

  if (follow_bottom) {
    first_visible_line = bottom_start;
  } else {
    if (total <= cap) {
      first_visible_line = 0;
      follow_bottom = true;
    } else if (first_visible_line > bottom_start) {
      first_visible_line = bottom_start;
      follow_bottom = true;
    }
  }

  const auto start_index = line_start_index(first_visible_line);
  const auto n = buffer.count();

  uint32_t px = area.x;
  uint32_t py = area.y;
  tr.set_pos(px, py);

  const auto gw = (tr.font.glyph_width + style.gap_x) * style.scale;
  const auto gh = line_height();

  size_t current_line = 0;

  real_cursor = target_cursor;

  for (size_t i = start_index; i < n; ++i) {
    char g = buffer[i];

    if (g == '\n') {
      ++current_line;
      if (current_line >= cap) break;

      draw_pos.x = 0;
      ++draw_pos.y;
      py += gh;
      tr.set_pos(px, py);
      continue;
    }

    auto next_x = px + (gw * draw_pos.x);
    // This causes a bug where we won't be scrolled all the way to the bottom,
    // because we are not counting the number of linewraps among the visible lines beforehand.
    // TODO: Fix this later
    if (next_x >= area.end_x()) {
      ++current_line;
      if (current_line >= cap) break;

      draw_pos.x = 0;
      ++draw_pos.y;
      py += gh;
      tr.set_pos(px, py);
    }

    if (i + 1 < n && draw_pos.y == target_cursor.y && buffer[i + 1] == '\n' &&
        draw_pos.x < real_cursor.x) {
      real_cursor.x = draw_pos.x;
    }

    if (draw_pos == real_cursor) {
      real_cursor_idx = i;
      tr.draw_glyph(g, true);
    } else {
      tr.draw_glyph(g);
    }

    ++draw_pos.x;
  }

  if (draw_pos.y == target_cursor.y && draw_pos.x <= target_cursor.x) {
    real_cursor.x = draw_pos.x;
    real_cursor_idx = buffer.count();
  }

  tr.draw_glyph(' ', draw_pos == real_cursor);
  buffer.move_to(real_cursor_idx);
}

void TextArea::scroll_up(size_t amount) {
  if (amount == 0) return;

  const auto total = count_lines();
  const auto cap = visible_line_capacity();

  if (cap == 0 || total <= cap) return;

  if (amount > first_visible_line) {
    first_visible_line = 0;
  } else {
    first_visible_line -= amount;
  }

  follow_bottom = false;
}

void TextArea::scroll_down(size_t amount) {
  if (amount == 0) return;

  const auto total = count_lines();
  const auto cap = visible_line_capacity();

  if (cap == 0) return;

  if (total <= cap) {
    first_visible_line = 0;
    follow_bottom = true;
    return;
  }

  const auto bottom_start = total - cap;
  auto new_first = first_visible_line + amount;

  if (new_first >= bottom_start) {
    first_visible_line = bottom_start;
    follow_bottom = true;
  } else {
    first_visible_line = new_first;
    follow_bottom = false;
  }
}

size_t TextArea::current_line_length() const noexcept {
  if (real_cursor.y == lines) { return buffer.count() - real_cursor_idx; }

  size_t count = real_cursor.x;
  for (size_t i = real_cursor_idx; i < buffer.count() && buffer[i] && buffer[i] != '\n';
       ++i, ++count) {}

  return count;
}

size_t TextArea::visible_line_capacity() const noexcept {
  if (area.h == 0) return 0;

  const auto lh = line_height();
  if (lh == 0) return 0;

  return static_cast<size_t>(area.h / lh);
}

size_t TextArea::count_lines() noexcept {
  const auto n = buffer.count();
  if (n == 0) {
    lines = 1;
    return lines;
  }

  size_t count = 1;
  for (size_t i = 0; i < n; ++i) {
    if (buffer[i] == '\n') { ++count; }
  }

  lines = count;
  return lines;
}

size_t TextArea::line_start_index(size_t line) const noexcept {
  if (line == 0) return 0;

  const auto n = buffer.count();
  size_t cur = 0;

  for (size_t i = 0; i < n; ++i) {
    if (buffer[i] == '\n') {
      ++cur;
      if (cur == line) { return i + 1; }
    }
  }
  return n;
}

size_t TextArea::line_height() const noexcept {
  return (tr.font.glyph_height + style.gap_y) * style.scale;
}

}  // namespace ui
