#include <cstdint>
#include <cstring>
#include <iterator>

// #include <kernel/log.hpp>
#include <kernel/panic.hpp>

#include "board/serial_desc.hpp"
#include "boot/boot_context.hpp"
#include "drv/global_core.hpp"
#include "hal/boot.hpp"
#include "hal/serial.hpp"
#include "kernel.hpp"
#include "memory/builtin/bm_heap.hpp"
#include "memory/heap.hpp"
#include "x86/common/board/pc_devices.hpp"
#include "x86/common/drv/register.hpp"
#include "x86/common/graphics/framebuffer.hpp"
#include "x86/common/input/keyboard.hpp"

using namespace x86;

// 4K aligned address after kernel
extern "C" char _ld_kernel_end_addr;

inline ::drv::serial::Port* find_console(::drv::GlobalCore& core) noexcept {
  using DescEntry = ::drv::GlobalCore::DescEntry;

  ::drv::serial::Port* p = core.find_if<::drv::serial::Port>(
      [](const DescEntry& de, void* iface, ::drv::DriverBase*) {
        (void)iface;
        if (de.type != ::drv::TypeTag::of<board::SerialDesc>()) { return false; }
        auto* sd = static_cast<const board::SerialDesc*>(de.ptr);
        if (!sd->name) { return false; }
        return std::strncmp(sd->name, "com1", 4) == 0;
      });

  return p;
}

void setup_boot_fb(kernel::KernelServices& kserv) {
  mb2::FramebufferTag fb_tag{};
  static graphics::Framebuffer boot_fb;

  if (mb2::load_tag<mb2::TagType::Framebuffer>(fb_tag)) {
    boot_fb = graphics::Framebuffer(fb_tag);

    if (boot_fb.valid()) { kserv.framebuffer = &boot_fb; }
  }
}

void make_mem_map(boot::BootContext& ctx) {
  static boot::MemoryRegion regions[16];
  size_t count = 0;

  mb2::MmapTag* mmap = nullptr;
  if (!mb2::load_tag<mb2::TagType::Mmap>(&mmap)) {
    ctx.memory_regions = 0;
    ctx.memory_map = nullptr;
    return;
  }

  auto* header = reinterpret_cast<mb2::TagHeader*>(reinterpret_cast<uint8_t*>(mmap) -
                                                   mb2::HeaderSize);

  auto* tag_begin = reinterpret_cast<uint8_t*>(header);
  auto* tag_end = tag_begin + header->size;

  auto* entry = mmap->entries;
  while (reinterpret_cast<uint8_t*>(entry) < tag_end) {
    using Entry = mb2::MmapTag::Entry;

    auto base = static_cast<uintptr_t>(entry->base_addr);
    auto len = static_cast<size_t>(entry->length);

    boot::MemoryRegionType type_out = boot::MemoryRegionType::Unknown;

    switch (entry->type) {
      case Entry::Type::AvailableRAM:
        type_out = boot::MemoryRegionType::Usable;
        break;
      case Entry::Type::ApciInfo:
        type_out = boot::MemoryRegionType::AcpiReclaimable;
        break;
      case Entry::Type::Reserved:
        type_out = boot::MemoryRegionType::Reserved;
        break;
      case Entry::Type::DefectiveRAM:
        type_out = boot::MemoryRegionType::Bad;
        break;
      default:
        type_out = boot::MemoryRegionType::Unknown;
        break;
    }

    if (type_out != boot::MemoryRegionType::Unknown && count < std::size(regions)) {
      regions[count++] = boot::MemoryRegion{type_out, base, len};
    }

    auto* bytes = reinterpret_cast<uint8_t*>(entry);
    entry = reinterpret_cast<Entry*>(bytes + mmap->entry_size);
  }

  ctx.memory_regions = count;
  ctx.memory_map = regions;
}

void make_basic_mem(boot::BootContext& ctx) {
  mb2::BasicMemInfoTag inf;
  if (load_tag<mb2::TagType::BasicMeminfo>(inf)) { ctx.upper_mem_kb = inf.mem_upper; }
}

extern "C" void kmain(uint32_t mb2_info_addr) {
  auto& core = ::drv::core();

  boot::BootContext ctx{};
  kernel::KernelServices serv{};

  ctx.arch = boot::ArchKind::X86;
  ctx.bootloader = boot::BootLoaderKind::Multiboot2;

  // preload all tags:
  (void)mb2::get_tag_map(mb2_info_addr);

  board::pc::register_board_devices(core);
  x86::drv::register_arch_drivers(core);
  core.bind_all();

  serv.keyboard = &x86::input::PS2Keyboard::get_instance();
  serv.serial = find_console(core);
  serv.serial->init();

  setup_boot_fb(serv);
  make_basic_mem(ctx);
  make_mem_map(ctx);
  ctx.ram_start_addr = reinterpret_cast<uintptr_t>(&_ld_kernel_end_addr);
  const char* str;
  if (mb2::get_cmdline(&str)) {
    ctx.cmdline = str;
  } else {
    ctx.cmdline = "<none>";
  }
  if (mb2::get_bootloader_name(&str))
    ctx.bootloader_name = str;
  else
    ctx.bootloader_name = "Unknown";

  ctx.ram_start_addr = 0x150000;
  auto* kernel_heap = mem::get_heap<mem::builtin::BmHeap>();
  mem::init_heap(kernel_heap, ctx.ram_start_addr, 32 * mem::MiB);
  mem::set_kernel_heap(*kernel_heap);

  kernel::Kernel kernel{serv, ctx};
  kernel.enter();
  panic("Unexpected kernel termination. %u", 1);
}
