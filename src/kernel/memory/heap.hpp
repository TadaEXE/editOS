#pragma once

#include <cstddef>
#include <cstdint>

#include "kernel/memory/byte_conversion.hpp"

namespace memory {

static constexpr uintptr_t align_up(uintptr_t v, size_t align) noexcept {
  return (v + align - 1) & ~(static_cast<uintptr_t>(align) - 1);
}

class Heap {
 public:
  virtual ~Heap() = default;
  virtual void* alloc(size_t size, size_t align = alignof(max_align_t)) noexcept = 0;
  virtual void free(void* ptr) noexcept = 0;
};

void init_heap(uintptr_t start, size_t size = 32 * MiB) noexcept;

void* alloc(size_t size, size_t align = alignof(max_align_t)) noexcept;

void free(void* ptr) noexcept;

}  // namespace memory
