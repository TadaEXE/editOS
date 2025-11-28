#pragma once
#include "hw/graphics/framebuffer.hpp"
#include "gfx/shapes.hpp"
#include <cstdint>

namespace gfx {

class Canvas {
public:
  explicit Canvas(hw::graphics::Framebuffer &fb) : fb(fb) {}

  uint32_t width() const noexcept { return fb.get_width(); }
  uint32_t height() const noexcept { return fb.get_height(); }

  void clear(uint32_t argb) noexcept { fb.clear(argb); }
  void put_pixel(uint32_t x, uint32_t y, uint32_t argb) noexcept {
    fb.put_pixel(x, y, argb);
  }
  void fill_rect(Rect rect, uint32_t argb) noexcept;
  void put_border(Rect rect, int16_t thickness, uint32_t argb) noexcept;

private:
  hw::graphics::Framebuffer &fb;
};
} // namespace gfx
