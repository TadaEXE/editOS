#include "arch/x86/graphics/framebuffer.hpp"

#include "gfx/color.hpp"

namespace x86::graphics {

void Framebuffer::clear(gfx::Color color) noexcept {
  if (!valid()) return;

  for (uint32_t y = 0; y < height; ++y) {
    auto* row = addr + y * pitch;
    auto* px = reinterpret_cast<uint32_t*>(row);
    for (uint32_t x = 0; x < width; ++x) {
      px[x] = color;
    }
  }
}

void Framebuffer::put_pixel(uint32_t x, uint32_t y, gfx::Color color) noexcept {
  if (!valid()) return;
  if (x >= width || y >= height) return;

  auto* row = addr + y * pitch;
  auto* px = reinterpret_cast<uint32_t*>(row);
  px[x] = color;
}
}  // namespace x86::graphics
