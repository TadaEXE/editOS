#pragma once

#include <cstdint>

#include "kernel/data_structures/flatmap.hpp"

namespace x86::multiboot2 {

enum class TagType : uint32_t {
  End = 0,

  Cmdline = 1,
  BootloaderName = 2,
  Module = 3,
  BasicMeminfo = 4,
  BootDevice = 5,
  Mmap = 6,
  Vbe = 7,
  Framebuffer = 8,
  ElfSections = 9,
  Apm = 10,

  Efi32SystemTable = 11,
  Efi64SystemTable = 12,

  Smbios = 13,
  AcpiV1RsdPtr = 14,
  AcpiV2RsdPtr = 15,

  Network = 16,
  EfiMemoryMap = 17,
  EfiBootServicesNotTerminated = 18,

  Efi32ImageHandle = 19,
  Efi64ImageHandle = 20,

  ImageLoadBaseAddress = 21

};

constexpr size_t TAG_COUNT = 22;

struct __attribute__((packed)) Tag {
  TagType type;
  uint32_t size;
};

struct __attribute__((packed)) FramebufferTag {

  TagType type;
  uint32_t size;
  uint64_t addr;
  uint32_t pitch;
  uint32_t width;
  uint32_t height;
  uint8_t bpp;
  uint8_t type_fb;
  uint16_t reserved;
};

struct FramebufferInfo {
  uint64_t addr;
  uint32_t pitch;
  uint32_t width;
  uint32_t height;
  uint8_t bpp;
  uint8_t type_fb;
};

struct __attribute__((packed)) StringOnlyTag {
  TagType type;
  uint32_t size;
  char str[];
};

ds::FlatMap<TagType, Tag*, TAG_COUNT>& get_tag_map(uint32_t mb2_info_addr = 0x0) noexcept;
bool get_boot_framebuffer(FramebufferInfo& out, uint32_t mb2_info_addr = 0x0);
bool get_cmdline(const char** cmdline, uint32_t mb2_info_addr = 0x0);
bool get_bootloader_name(const char** bootloader_name, uint32_t mb2_info_addr = 0x0);

}  // namespace x86::multiboot2
