#include "kernel/memory/heap.hpp"

namespace memory {

namespace {
Heap* global_kernel_heap = nullptr;
}

void set_kernel_heap(Heap& heap) noexcept {
  global_kernel_heap = &heap;
}

Heap* kernel_heap() noexcept {
  return global_kernel_heap;
}

void* alloc(size_t size, size_t align) noexcept {
  return global_kernel_heap ? global_kernel_heap->alloc(size, align) : nullptr;
}

void free(void* ptr) noexcept {
  if (global_kernel_heap) global_kernel_heap->free(ptr);
}

}  // namespace memory
