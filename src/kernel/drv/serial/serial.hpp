#pragma once

#include <cstddef>
#include <cstdint>

namespace drv::serial {

class Port {
 public:
  virtual ~Port() = default;
  virtual void init() noexcept = 0;
  virtual bool write_byte(uint8_t b) noexcept = 0;
  virtual bool read_byte(uint8_t& b) noexcept = 0;
  virtual bool write(const uint8_t* data, size_t len) noexcept = 0;
  virtual void flush() noexcept = 0;
};

}  // namespace drv::serial
