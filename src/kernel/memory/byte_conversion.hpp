#pragma once

#include <cstdint>

namespace memory {

inline constexpr uintptr_t KiB = 1024ULL;
inline constexpr uintptr_t MiB = 1024ULL * KiB;
inline constexpr uintptr_t GiB = 1024ULL * MiB;

namespace {
inline constexpr uintptr_t save_div(uintptr_t bytes, uintptr_t unit) {
  if (!unit) return 0;
  auto mod = bytes % unit;
  return mod ? bytes / unit + 1 : bytes / unit;
}
}  // namespace

inline constexpr uintptr_t B_to_KiB(uintptr_t bytes) {
  return save_div(bytes, KiB);
}

inline constexpr uintptr_t B_to_MiB(uintptr_t bytes) {
  return save_div(bytes, MiB);
}

inline constexpr uintptr_t B_to_GiB(uintptr_t bytes) {
  return save_div(bytes, GiB);
}

inline constexpr uintptr_t KiB_to_B(uintptr_t kib) {
  return kib * KiB;
}

inline constexpr uintptr_t KiB_to_MiB(uintptr_t kib) {
  return save_div(kib, KiB);
}

inline constexpr uintptr_t KiB_to_GiB(uintptr_t kib) {
  return save_div(kib, MiB);
}

inline constexpr uintptr_t MiB_to_B(uintptr_t mib) {
  return mib * MiB;
}

inline constexpr uintptr_t MiB_to_KiB(uintptr_t mib) {
  return mib * KiB;
}

inline constexpr uintptr_t MiB_to_GiB(uintptr_t mib) {
  return save_div(mib, KiB);
}

inline constexpr uintptr_t GiB_to_B(uintptr_t gib) {
  return gib * GiB;
}

inline constexpr uintptr_t GiB_to_KiB(uintptr_t gib) {
  return gib * MiB;
}

inline constexpr uintptr_t GiB_to_MiB(uintptr_t gib) {
  return gib * KiB;
}

}  // namespace memory
