#pragma once

#include <cstdint>

#include "gfx/color.hpp"

namespace hal {

class Framebuffer {
 public:
  virtual ~Framebuffer() = default;

  virtual bool valid() const noexcept = 0;
  virtual void clear(gfx::Color color) noexcept = 0;
  virtual void put_pixel(uint32_t x, uint32_t y, gfx::Color color) noexcept = 0;
  virtual uint32_t get_width() const noexcept = 0;
  virtual uint32_t get_height() const noexcept = 0;
};

}  // namespace hal
