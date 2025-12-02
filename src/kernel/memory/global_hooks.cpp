#include "kernel/memory/heap.hpp"
#include "kernel/panic.hpp"

void* operator new(size_t sz) {
  if (void* p = mem::alloc(sz)) { return p; }
  panic("Out of memory");
}

void* operator new[](size_t sz) {
  if (void* p = mem::alloc(sz)) { return p; }
  for (;;) {}
}

void operator delete(void* ptr) noexcept {
  mem::free(ptr);
}

void operator delete[](void* ptr) noexcept {
  mem::free(ptr);
}

// Some ABIs also call sized delete; make them aliases.
void operator delete(void* ptr, size_t) noexcept {
  mem::free(ptr);
}

void operator delete[](void* ptr, size_t) noexcept {
  mem::free(ptr);
}
