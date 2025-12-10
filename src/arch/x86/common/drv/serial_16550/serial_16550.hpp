#pragma once

#include <array>

#include "board/serial_desc.hpp"
#include "drv/driver.hpp"
#include "drv/serial/serial.hpp"
#include "x86/common/io/ports.hpp"

namespace x86::drv {

class Serial16550Impl final : public ::drv::serial::Port {
 public:
  Serial16550Impl& operator=(const Serial16550Impl&) = delete;
  Serial16550Impl& operator=(Serial16550Impl&&) = delete;
  Serial16550Impl(const Serial16550Impl&) = delete;
  Serial16550Impl(Serial16550Impl&&) = default;

  Serial16550Impl() = default;

  explicit Serial16550Impl(const board::SerialDesc& desc) noexcept
      : desc(&desc),
        data(desc.io_base + 0),
        int_en(desc.io_base + 1),
        fifo_ctrl(desc.io_base + 2),
        line_ctrl(desc.io_base + 3),
        modem_ctrl(desc.io_base + 4),
        line_status(desc.io_base + 5) {}

  void init() noexcept override;
  bool write_byte(uint8_t b) noexcept override;
  bool read_byte(uint8_t& b) noexcept override;
  bool write(const uint8_t* data, size_t len) noexcept override;

  void flush() noexcept override {}

  const board::SerialDesc& get_desc() const noexcept { return *desc; }

 private:
  void write_raw(uint8_t b) const noexcept;
  const board::SerialDesc* desc;
  bool inited{false};

  io::Port8 data;
  io::Port8 int_en;
  io::Port8 fifo_ctrl;
  io::Port8 line_ctrl;
  io::Port8 modem_ctrl;
  io::Port8 line_status;
};

class Serial16550Driver : public ::drv::Driver<::drv::serial::Port, board::SerialDesc> {
 public:
  Serial16550Driver() = default;

  bool match(const board::SerialDesc& desc) const noexcept override {
    return desc.io_base == 0x3F8 || desc.io_base == 0x2F8;
  }

  ::drv::serial::Port* probe(const board::SerialDesc& desc) noexcept override {
    for (size_t i = 0; i < max_instances; ++i) {
      if (!used[i]) {
        used[i] = true;
        new (instances.data() + i) Serial16550Impl(desc);
        return &instances[i];
      }
    }

    return nullptr;
  }

 private:
  static constexpr size_t max_instances = 4;
  std::array<Serial16550Impl, max_instances> instances{};
  bool used[max_instances]{};
};
}  // namespace x86::drv
