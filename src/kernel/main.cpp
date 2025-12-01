#include "gfx/canvas.hpp"
#include "gfx/shapes.hpp"
#include "gfx/text/text.hpp"
#include "hal/boot.hpp"
#include "hal/keyboard.hpp"
#include "input/keymap_us.hpp"
#include "kernel/log.hpp"
#include "kernel/memory/byte_conversion.hpp"
#include "kernel/memory/heap.hpp"
#include "kernel/panic.hpp"
#include "logging/backend/serial.hpp"
#include "ui/core/window.hpp"

namespace {
bool kernel_fuse() {
  static bool fuse{false};
  if (fuse) {
    log_msg("Tried to enter kernel (hal::enter_kernel()) more than once!");
    return false;
  }
  return true;
}

void setup_logging(boot::BootContext& ctx) {
  if (ctx.system_serial_bus) {
    static logging::backend::SerialSink serial_sink(*ctx.system_serial_bus);
    logging::backend::set_sink(&serial_sink);
    log_msg("");
    log_msg("");
    log_msg("");
  }
}
}  // namespace

[[noreturn]] void hal::enter_kernel(boot::BootContext& ctx) {
  if (!kernel_fuse()) panic("Entry fuse blew");

  setup_logging(ctx);
  log_msg("editOS kernel entered...");
  log_msg("Booted by %s", ctx.bootloader_name);
  log_msg("Bootoptions: %s", ctx.cmdline);
  log_msg("%d KiB available in upper memory", ctx.upper_mem_kb);

  if (ctx.memory_map && ctx.memory_regions) {
    log_msg("Found memory map (%d entries):", ctx.memory_regions);
    for (size_t i = 0; i < ctx.memory_regions; ++i) {
      auto e = ctx.memory_map[i];
      log_msg("  %d MiB at %x [%s]", memory::B_to_MiB(e.length), e.addr,
              boot::MemoryRegionTypeName(e.type));
    }
    memory::init_heap(ctx.ram_start_addr, 32 * memory::MiB);
    log_msg("Kernel heap (%d MiB) initialized at %p", 32, ctx.ram_start_addr);
  }

  hal::Framebuffer* fb = ctx.boot_framebuffer;
  if (!fb) { panic("No framebuffer provided by bootloader. Abort!"); }

  gfx::Canvas can(*fb);
  can.clear(0xFF202040);
  gfx::Rect r{10, 10, 100, 100};
  auto r2 = r - 5;

  ui::Window win{r};
  win.draw(can);

  gfx::text::Style style{gfx::Color::Black(), gfx::Color::White(), false, 1, 0};
  gfx::text::TextRenderer tr{can, style};
  tr.set_pos(200, 200);

  auto& kb = hal::keyboard();
  for (;;) {
    hal::KeyEvent ev{};
    if (kb.poll(ev)) {
      char c;
      if (input::key_event_to_char(ev, c)) { tr.draw_glyph(c); }
    }
  }
}
