#include "x86/common/drv/register.hpp"

#include "board/serial_desc.hpp"
#include "drv/driver.hpp"
#include "drv/serial/serial.hpp"
#include "serial_16550/serial_16550.hpp"
#include "x86/common/drv/serial_16550/serial_16550.hpp"

namespace x86::drv {

void register_arch_drivers(::drv::GlobalCore& core) noexcept {
  static x86::drv::Serial16550Driver serial_driver{};
  core.register_driver(serial_driver);
}

}  // namespace x86::drv
