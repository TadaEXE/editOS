#include "board/serial_desc.hpp"
#include "drv/global_core.hpp"

namespace board::pc {

static constexpr SerialDesc serials[] = {
    {"com1", 0x3F8, 4},
    {"com2", 0x2F8, 3},
};

const SerialDesc* serial_begin() noexcept {
  return &serials[0];
}

const SerialDesc* serial_end() noexcept {
  return &serials[0] + 2;
}

void register_board_devices(drv::GlobalCore& core) noexcept {
  for (const auto& s : serials) {
    core.add_desc(s);
  }
}

}  // namespace board::pc
