#pragma once
#include <cstdint>

#include "kernel/logging/logging.hpp"

namespace gfx {

struct Line : public logging::Loggable {
  uint32_t s;
  uint32_t e;
  uint32_t w;
};

struct Rect : public logging::Loggable {
  uint32_t x;
  uint32_t y;
  uint32_t w;
  uint32_t h;

  Rect(const Rect&) = default;
  Rect(Rect&&) = default;
  Rect& operator=(const Rect&) = default;
  Rect& operator=(Rect&&) = default;

  Rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h) : x(x), y(y), w(w), h(h) {}

  uint32_t end_x() { return x + w; }

  uint32_t end_y() { return y + h; }

  Rect& operator-=(int32_t thickness);
  Rect& operator+=(int32_t thickness);

  void log_self() const noexcept override { log_obj<Rect>(x, y, w, h); }

  static constexpr const char* fmt() noexcept { return "{x=%u, y=%u, w=%u, h=%u}"; }

  /// Inclusive start, exclusive end: [x, x+w), [y, y+h)
  constexpr bool is_inbounds(uint32_t px, uint32_t py) const {
    return (px >= x && px < x + w) && (py >= y && py < y + h);
  }
};

Rect operator+(const Rect& r, int32_t thickness);
Rect operator-(const Rect& r, int32_t thickness);

struct Sphere {
  uint32_t x;
  uint32_t y;
  uint32_t r;
};

}  // namespace gfx
