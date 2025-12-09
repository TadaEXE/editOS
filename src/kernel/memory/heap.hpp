#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>

#include "memory/byte_conversion.hpp"

namespace mem {

static constexpr uintptr_t align_to(uintptr_t v, size_t align) noexcept {
  return (v + align - 1) & ~(static_cast<uintptr_t>(align) - 1);
}

class Heap {
 public:
  enum class Named : uint16_t {
    Kernel = 0xFFFF,
  };

  virtual ~Heap() = default;
  virtual void init(uintptr_t addr, size_t size = 32 * MiB) noexcept = 0;
  virtual void* alloc(size_t size, size_t align = alignof(max_align_t)) noexcept = 0;
  virtual void free(void* ptr) noexcept = 0;
};

template <typename T>
concept HeapInstanceKey = std::same_as<T, uint16_t> || std::same_as<T, Heap::Named>;

template <typename H, auto Key = Heap::Named::Kernel>
  requires std::derived_from<H, Heap>
H* get_heap() noexcept {
  static H heap;
  return &heap;
}

Heap* kernel_heap() noexcept;
void set_kernel_heap(Heap& heap) noexcept;

template <typename H>
  requires std::derived_from<H, Heap>
H* init_heap(H* heap, uintptr_t addr, size_t size = 32 * MiB) noexcept {
  heap->init(addr, size);
  return heap;
}

void* alloc(size_t size, size_t align = alignof(max_align_t)) noexcept;

template <typename T>
T* alloc(size_t count, size_t align = alignof(T)) noexcept {
  return reinterpret_cast<T*>(alloc(sizeof(T) * count, align));
}

void free(void* ptr) noexcept;

}  // namespace mem
