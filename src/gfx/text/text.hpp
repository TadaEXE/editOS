#pragma once
#include <cstdint>

#include "gfx/canvas.hpp"
#include "gfx/color.hpp"
#include "gfx/text/bitmap_font.hpp"

namespace gfx::text {

struct Style {
  Color fg;
  Color bg;
  bool draw_bg;
  uint32_t scale{1};
  uint32_t gap{0};
};

class TextRenderer {
 public:
  TextRenderer(Canvas& canvas, Style style, BitmapFont& font)
      : canvas(canvas), style(style), font(font) {}
  TextRenderer(Canvas& canvas, Style style)
      : canvas(canvas), style(style), font(builtin_font()) {}

  void draw_glyph(char c);
  void draw_glyph(char c, uint32_t x, uint32_t y);

  void draw_text(const char* text);
  void draw_text(const char* text, uint32_t x, uint32_t y);

  void set_pos(uint32_t x, uint32_t y);

  void set_style(Style style) { this->style = style; }

 private:
  Canvas& canvas;
  Style style;
  const BitmapFont& font;
  uint32_t last_x{0};
  uint32_t last_y{0};
};

}  // namespace gfx::text
