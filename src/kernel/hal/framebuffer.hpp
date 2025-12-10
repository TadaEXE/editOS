#pragma once

#include <cstdint>

#include "logging/logging.hpp"

namespace hal {

class Framebuffer : public logging::Loggable {
 public:
  virtual ~Framebuffer() = default;

  virtual bool valid() const noexcept = 0;
  virtual uint32_t get_width() const noexcept = 0;
  virtual uint32_t get_height() const noexcept = 0;
  virtual uint32_t get_bpp() const noexcept = 0;
  virtual uint32_t get_pitch() const noexcept = 0;
  virtual uint8_t get_type() const noexcept = 0;
  virtual uint8_t* begin() noexcept = 0;
  virtual uint8_t* end() noexcept = 0;

  static constexpr const char* fmt() noexcept {
    return "{w=%u, h=%u, bpp=%u, pitch=%u, v=%u}";
  }

  void log_self() const noexcept override {
    log_obj<Framebuffer>(get_width(), get_height(), get_bpp(), get_pitch(), valid());
  }
};

}  // namespace hal
