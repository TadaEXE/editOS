#pragma once
#include <cstddef>
#include <type_traits>

#include "kernel/memory/data_view.hpp"
#include "kernel/memory/heap.hpp"

namespace memory {

/// @brief Stack-based owing implementation of non-owning DataView
template <typename T, size_t Size>
  requires std::is_trivially_copyable_v<T>
class StackStorage : public DataView<T> {
 public:
  StackStorage() : DataView<T>(buffer, Size) {}

 private:
  T buffer[Size];
};

/// @brief Heap-based owing implementation of non-owning DataView
template <typename T>
  requires std::is_trivially_copyable_v<T>
class HeapStorage : public DataView<T> {
 public:
  explicit HeapStorage(size_t initial_capacity = 64)
      : capacity(initial_capacity ? initial_capacity : 1),
        buffer(static_cast<T*>(memory::alloc(sizeof(T) * capacity, alignof(T)))),
        DataView<T>(nullptr, capacity) {}

protected:
  bool grow_to(size_t len) noexcept override {
    if (len <= capacity) {
      this->length = len;
      return true;
    }

  }

 private:
  T* buffer;
  size_t capacity;
};
}  // namespace memory
