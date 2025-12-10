#pragma once

#include <cstdint>

#include "boot/multiboot2.hpp"
#include "hal/framebuffer.hpp"

namespace x86::graphics {

class Framebuffer : public hal::Framebuffer {
 public:
  Framebuffer() = default;

  Framebuffer(const Framebuffer&) = delete;
  Framebuffer(Framebuffer&&) = delete;
  Framebuffer& operator=(const Framebuffer&) = default;
  Framebuffer& operator=(Framebuffer&&) = default;

  explicit Framebuffer(const mb2::FramebufferTag& info)
      : addr(reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(info.addr))),
        pitch(info.pitch),
        width(info.width),
        height(info.height),
        bpp(info.bpp),
        type_fb(info.type_fb) {}

  bool valid() const noexcept override {
    return addr != nullptr && bpp == 32 && width > 0 && height > 0 && pitch >= width * 4;
  }

  uint32_t get_width() const noexcept override { return width; }

  uint32_t get_height() const noexcept override { return height; }

  uint32_t get_bpp() const noexcept override { return bpp; }

  uint32_t get_pitch() const noexcept override { return pitch; }

  uint8_t get_type() const noexcept override {return type_fb; }

  uint8_t* begin() noexcept override { return addr; }

  uint8_t* end() noexcept override { return addr + pitch * height; }

 private:
  uint8_t* addr{nullptr};
  uint32_t pitch{0};
  uint32_t width{0};
  uint32_t height{0};
  uint8_t bpp{0};
  uint8_t type_fb{0};
};
}  // namespace x86::graphics
