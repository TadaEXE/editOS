#include "kernel/boot/multiboot2.hpp"

#include <cstdint>
#include <cstring>

namespace mb2 {

ctr::FlatMap<TagType, TagHeader*, TAG_COUNT>& get_tag_map(
    uint32_t mb2_info_addr) noexcept {
  static ctr::FlatMap<TagType, TagHeader*, TAG_COUNT> tag_map{};
  if (!tag_map.empty() || mb2_info_addr == 0x0) return tag_map;

  auto* base = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(mb2_info_addr));
  TagHeader* tag = reinterpret_cast<TagHeader*>(base + 8);

  while (tag->type != TagType::End) {
    if (static_cast<uint32_t>(tag->type) > 0 &&
        static_cast<uint32_t>(tag->type) < TAG_COUNT) {
      tag_map.insert(tag->type, tag);
    }

    // Move to next tag (8-byte aligned)
    uint8_t* next = reinterpret_cast<uint8_t*>(tag) + tag->size;
    uintptr_t aligned =
        (reinterpret_cast<uintptr_t>(next) + 7u) & ~static_cast<uintptr_t>(7u);
    tag = reinterpret_cast<TagHeader*>(aligned);
  }

  return tag_map;
}

bool get_cmdline(const char** cmdline, uint32_t mb2_info_addr) {
  StringOnlyTag* sot;
  if (load_tag<TagType::Cmdline, StringOnlyTag>(&sot, mb2_info_addr)) {
    log_msg("get_bl: %s", sot->str);
    *cmdline = sot->str;
    return true;
  }

  *cmdline = nullptr;
  return false;
}

bool get_bootloader_name(const char** bootloader_name, uint32_t mb2_info_addr) {
  StringOnlyTag* sot;
  if (load_tag<TagType::BootloaderName, StringOnlyTag>(&sot, mb2_info_addr)) {
    log_msg("get_bl: %s", sot->str);
    *bootloader_name = sot->str;
    return true;
  }

  *bootloader_name = nullptr;
  return false;
}

}  // namespace mb2
