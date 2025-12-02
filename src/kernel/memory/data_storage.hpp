#pragma once

#include <cstddef>
#include <cstring>
#include <type_traits>

#include "kernel/memory/data_view.hpp"
#include "kernel/memory/heap.hpp"
#include "math/bit_logic.hpp"

namespace mem {

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
      : capacity(bits::oiz(bits::clp2(initial_capacity))),
        buffer(mem::alloc<T>(sizeof(T) * capacity, alignof(T))),
        DataView<T>(buffer, capacity) {}

 protected:
  bool grow_to(size_t len) noexcept override {
    if (len <= capacity) {
      this->length = len;
      return true;
    }

    auto nc = bits::clp2(capacity + 1);
    T* tmp = alloc<T>(sizeof(T) * nc, alignof(T));
    if (!tmp) return false;
    memcpy(tmp, buffer, this->length);

    capacity = nc;
    buffer = tmp;
    return true;
  }

 private:
  T* buffer;
  size_t capacity;
};
}  // namespace mem
