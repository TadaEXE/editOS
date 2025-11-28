#include "gfx/canvas.hpp"
#include "gfx/shapes.hpp"
#include <cstdint>

namespace gfx {

void Canvas::fill_rect(Rect r, uint32_t argb) noexcept {
  const auto xmax = r.x + r.w;
  const auto ymax = r.y + r.h;
  for (uint32_t yy = r.y; yy < ymax; ++yy) {
    for (uint32_t xx = r.x; xx < xmax; ++xx) {
      fb.put_pixel(xx, yy, argb);
    }
  }
}
} // namespace gfx
