#include <cstdint>

#include "gfx/canvas.hpp"
#include "gfx/shapes.hpp"
#include "gfx/text/text.hpp"
#include "hal/boot.hpp"
#include "hal/keyboard.hpp"
#include "input/keymap_us.hpp"
#include "kernel/logging/logging.hpp"
#include "logging/backend/serial.hpp"
#include "ui/core/window.hpp"

namespace {
bool kernel_fuse() {
  static bool fuse{false};
  if (fuse) {
    log::msg("Tried to enter kernel (hal::enter_kernel()) more than once!");
    return false;
  }
  return true;
}

void setup_logging(hal::BootContext& ctx) {
  if (ctx.system_serial_bus) {
    static log::backend::SerialSink serial_sink(*ctx.system_serial_bus);
    log::backend::set_sink(&serial_sink);
    log::msg("");
    log::msg("");
    log::msg("");
  }
}
}  // namespace

void hal::enter_kernel(hal::BootContext& ctx) {
  if (!kernel_fuse()) return;
  setup_logging(ctx);
  log::msg("editOS kernel entered...");

  hal::Framebuffer* fb = ctx.boot_framebuffer;
  if (!fb) {
    log::msg("No framebuffer provided by bootloader. Abort!");
    return;
  }
  gfx::Canvas can(*fb);
  can.clear(0xFF202040);
  gfx::Rect r{10, 10, 100, 100};
  auto r2 = r - 5;

  ui::Window win{r};
  win.draw(can);

  log::msg("Booted by %s", ctx.bootloader_name);
  log::msg("Bootoptions: %s", ctx.cmdline);

  gfx::text::Style style{gfx::Color::Black(), gfx::Color::White(), false, 1, 0};
  gfx::text::TextRenderer tr{can, style};
  tr.set_pos(200, 200);

  auto& kb = hal::keyboard();
  for (;;) {
    hal::KeyEvent ev{};
    if (kb.poll(ev)) {
      char c;
      if (input::key_event_to_char(ev, c)) {
        tr.draw_glyph(c);
      }
    }
  }
}
