#include "ui/core/text_output.hpp"

namespace ui {

void TextOutput::put_char(char c) {
  buffer.insert(&c);
}

void TextOutput::move_cursor(gfx::Point np) {
  buffer.move_to(np.x);
}

void TextOutput::update() {
  canvas.clear(gfx::Color::Gray());
  this->draw(canvas);
  tr.set_pos(this->rect.x, this->rect.y);
  for (size_t i = 0; i < buffer.count(); ++i) {
    if (buffer[i] == '\n') { tr.set_pos(this->rect.x, this->rect.y); }
    tr.draw_glyph(buffer[i]);
  }
}

}  // namespace ui
