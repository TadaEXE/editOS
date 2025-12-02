#include "kernel/memory/heap.hpp"

#include <cstddef>
#include <cstdint>

namespace mem {

namespace {
constexpr size_t HEAP_SIZE = 1024 * 1024;

alignas(16) uint8_t heap_buffer[HEAP_SIZE];
size_t heap_offset = 0;

// align value "v" up to "align" (power of two)
constexpr uintptr_t align_up(uintptr_t v, size_t align) noexcept {
  return (v + align - 1) & ~(static_cast<uintptr_t>(align) - 1);
}
}  // namespace

void* alloc(size_t size, size_t align) noexcept {
  if (align < alignof(max_align_t)) {
    align = alignof(max_align_t);
  }

  const uintptr_t base = reinterpret_cast<uintptr_t>(heap_buffer);
  const uintptr_t current = base + heap_offset;

  const uintptr_t aligned = align_up(current, align);
  const uintptr_t new_end = aligned + size;

  if (new_end > base + HEAP_SIZE) {
    // Out of memory – we'll handle this in global new below.
    return nullptr;
  }

  heap_offset = static_cast<size_t>(new_end - base);
  return reinterpret_cast<void*>(aligned);
}

void free(void* /*ptr*/) noexcept {
  // No-op for bump allocator.
  // You could add debug bookkeeping here if you want.
}

void reset() noexcept {
  heap_offset = 0;
}


}  // namespace mem

