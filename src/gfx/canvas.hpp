#pragma once
#include <cstdint>

#include "gfx/color.hpp"
#include "gfx/shapes.hpp"
#include "hal/framebuffer.hpp"

namespace gfx {

class Canvas {
 public:
  explicit Canvas(hal::Framebuffer& fb) : fb(fb) {}

  uint32_t width() const noexcept { return fb.get_width(); }

  uint32_t height() const noexcept { return fb.get_height(); }

  void clear(Color color) noexcept { fb.clear(color); }

  void draw_pixel(uint32_t x, uint32_t y, Color color) noexcept;

  void draw_rect(Rect rect, Color color) noexcept;

  void draw_border(Rect outer, Rect inner, Color color) noexcept;
  void draw_border(Rect rect, int16_t thickness, Color color) noexcept;

  // void draw_char(uint32_t x, uint32_t y, char c, const text::Style& style) noexcept;
  // void draw_text(uint32_t x, uint32_t y, const char* text,
  //                const text::Style& style) noexcept;

 private:
  hal::Framebuffer& fb;
};
}  // namespace gfx
