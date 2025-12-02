#include "ui/core/text_output.hpp"

namespace ui {

void TextOutput::put_char(char c) {}

void TextOutput::move_cursor(gfx::Point& np) {
  if (!rect.is_inbounds(np)) {
    gfx::Point ib{};
    if (np.x < rect.x) { ib.x = rect.x; }
    if (np.x > rect.end_x()) { ib.x = rect.end_x(); }
    if (np.y < rect.y) { ib.y = rect.y; }
    if (np.y > rect.end_y()) { ib.y = rect.end_y(); }
  }
}

}  // namespace ui
