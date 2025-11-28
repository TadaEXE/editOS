#include "ui/window.hpp"
#include "gfx/canvas.hpp"

namespace ui {

void Window::draw(gfx::Canvas &canvas) noexcept {
}

void Window::set_position(uint32_t x, uint32_t y) noexcept {
  this->x = x;
  this->y = y;
  rect.x = x;
  rect.y = y;
}
} // namespace ui
