#include "kernel/memory/builtin/bm_heap.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "kernel/panic.hpp"

namespace memory::builtin {

bool BmHeap::add_block(uintptr_t addr, size_t size) noexcept {
  return add_block(addr, size, default_div_size);
}

bool BmHeap::add_block(uintptr_t addr, size_t size, size_t div_size) noexcept {
  if (!addr || size <= sizeof(HeapBlock) + div_size) return false;

  auto* block = reinterpret_cast<HeapBlock*>(addr);

  size_t space = size - sizeof(HeapBlock);
  size_t div_count = space / (div_size + 1);

  if (div_count == 0) return false;

  block->div_count = div_count;
  block->div_size = div_size;
  block->remaining = div_count * div_size;

  auto* bitmap = bitmap_for(block);
  memset(bitmap, 0, div_count);

  block->next = front;
  front = block;
  return true;
}

void BmHeap::init(uintptr_t addr, size_t size) noexcept {
  if (!add_block(addr, size)) { panic("Heap initialization failed"); }
}

void* BmHeap::alloc(size_t size, size_t align) noexcept {
  if (!size) return nullptr;
  if (align < alignof(max_align_t)) { align = alignof(max_align_t); }

  for (HeapBlock* block = front; block != nullptr; block = block->next) {
    if (block->remaining < size) continue;

    size_t needed_divs = (size + block->div_size - 1) / block->div_size;

    auto* bitmap = bitmap_for(block);
    auto* data = data_begin(block);

    size_t run_len = 0;
    size_t run_start = 0;

    for (size_t i = 0; i < block->div_count; ++i) {
      if (bitmap[i] != 0) {
        run_len = 0;
        continue;
      }

      if (run_len == 0) run_start = i;
      ++run_len;

      if (run_len >= needed_divs) {
        auto base_addr = reinterpret_cast<uintptr_t>(data) + run_start * block->div_size;

        auto id = next_id();
        memset(bitmap + run_start, id, needed_divs);

        block->remaining -= needed_divs * block->div_size;
        return reinterpret_cast<void*>(base_addr);
      }
    }
  }

  return nullptr;
}

void BmHeap::free(void* ptr) noexcept {
  if (!ptr) return;

  auto* p = reinterpret_cast<uint8_t*>(ptr);

  for (HeapBlock* block = front; block != nullptr; block = block->next) {
    auto* begin = data_begin(block);
    auto* end = data_end(block);

    if (p < begin || p >= end) continue;

    size_t offset = static_cast<size_t>(p - begin);
    size_t idx = offset / block->div_size;

    auto* bitmap = bitmap_for(block);
    auto id = bitmap[idx];
    if (id == 0) { panic("Double free"); }

    size_t freed_divs = 0;
    for (size_t i = idx; i < block->div_count && bitmap[i] == id; ++i) {
      bitmap[i] = 0;
      ++freed_divs;
    }

    block->remaining += freed_divs * block->div_size;
    return;
  }
}

}  // namespace memory::builtin
