#pragma once

#include "hw/boot/multiboot2.hpp"
#include <cstdint>

namespace hw::graphics {

class Framebuffer {
public:
  Framebuffer() = default;

  explicit Framebuffer(const multiboot2::FramebufferInfo &info)
      : addr(reinterpret_cast<uint8_t *>(static_cast<uintptr_t>(info.addr))),
        pitch(info.pitch), width(info.width), height(info.height),
        bpp(info.bpp), type_fb(info.type_fb) {}

  bool valid() const noexcept {
    return addr != nullptr && bpp == 32 && width > 0 && height > 0 &&
           pitch >= width * 4;
  }

  void clear(uint32_t argb) noexcept;
  void put_pixel(uint32_t x, uint32_t y, uint32_t argb) noexcept;

  uint32_t get_width() const noexcept { return width; }
  uint32_t get_height() const noexcept { return height; }

private:
  uint8_t *addr{nullptr};
  uint32_t pitch{0};
  uint32_t width{0};
  uint32_t height{0};
  uint8_t bpp{0};
  uint8_t type_fb{0};
};
} // namespace hw::graphics
