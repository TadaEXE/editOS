#include "arch/x86/boot/multiboot2.hpp"

#include <cstdint>
#include <cstring>

namespace x86::multiboot2 {

ds::FlatMap<TagType, Tag*, TAG_COUNT>& get_tag_map(uint32_t mb2_info_addr) noexcept {
  static ds::FlatMap<TagType, Tag*, TAG_COUNT> tag_map{};
  if (!tag_map.empty() || mb2_info_addr == 0x0) return tag_map;

  auto* base = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(mb2_info_addr));
  Tag* tag = reinterpret_cast<Tag*>(base + 8);

  while (tag->type != TagType::End) {
    if (static_cast<uint32_t>(tag->type) > 0 && static_cast<uint32_t>(tag->type) < TAG_COUNT) {
      tag_map.insert(tag->type, tag);
    }

    // Move to next tag (8-byte aligned)
    uint8_t* next = reinterpret_cast<uint8_t*>(tag) + tag->size;
    uintptr_t aligned = (reinterpret_cast<uintptr_t>(next) + 7u) & ~static_cast<uintptr_t>(7u);
    tag = reinterpret_cast<Tag*>(aligned);
  }

  return tag_map;
}

bool get_cmdline(const char** cmdline, uint32_t mb2_info_addr) {
  if (auto* tag = get_tag_map(mb2_info_addr).find(TagType::Cmdline)) {
    auto* sot = reinterpret_cast<StringOnlyTag*>(*tag);
    *cmdline = sot->str;
    return true;
  }

  *cmdline = nullptr;
  return false;
}

bool get_bootloader_name(const char** bootloader_name, uint32_t mb2_info_addr) {
  if (auto* tag = get_tag_map(mb2_info_addr).find(TagType::BootloaderName)) {
    auto* sot = reinterpret_cast<StringOnlyTag*>(*tag);
    *bootloader_name = sot->str;
    return true;
  }

  *bootloader_name = nullptr;
  return false;
}

bool get_boot_framebuffer(FramebufferInfo& out, uint32_t mb2_info_addr) {
  if (auto* tag = get_tag_map(mb2_info_addr).find(TagType::Framebuffer)) {
    auto* fb = reinterpret_cast<FramebufferTag*>(*tag);

    out.addr = fb->addr;
    out.pitch = fb->pitch;
    out.width = fb->width;
    out.height = fb->height;
    out.bpp = fb->bpp;
    out.type_fb = fb->type_fb;
    return true;
  }

  return false;
}

}  // namespace x86::multiboot2
