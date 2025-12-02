#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "kernel/memory/heap.hpp"
#include "math/bit_logic.hpp"

namespace ctr {

template <typename T, size_t GapSize = 32>
class GapBuffer {
 public:
  void insert(const T& item) {
    if (gap_width() > 0) {
      *gb++ = item;
      ++length;
    }

    grow();
    insert(item);
  }

  void move_left(uintptr_t count = 1) {
    if (!count) return;

    if (gb > begin) {
      shift(gb - count, count, ge - count);
      gb -= count;
      ge -= count;
    }
  }

  void delete_left(uintptr_t count = 1) {
    if (!count) return;

    auto t = gb;
    gb = gb - count > begin ? gb - count : begin;
    length -= t - gb;
  }

  void move_right(uintptr_t count = 1) {
    if (!count) return;

    if (ge < begin + length) {
      shift(ge + count, count, gb);
      gb += count;
      gb += count;
    }
  }

  void delete_right(uintptr_t count = 1) {
    if (!count) return;

    auto t = ge;
    ge = ge + count < begin + capacity ? ge + count : begin + capacity;
    length -= ge - t;
  }

  void move_to(uintptr_t index) {
    if (!index) return;

    auto w = gap_width();
    if (ge + w + index) {
      gb = begin + index;
      ge = gb + w;
    }
  }

  uintptr_t cursor_pos() { return gb - begin; }

 private:
  // Assumes memory is already allocated
  void shift(T* src, size_t length, T* dest) {
    for (size_t i = 0; i < length; ++i) {
      dest[i] = src[i];
    }
  }

  // Assumes memory is already allocated
  void shift(T* src, T* stop, T* dest) {
    for (size_t i = 0; src + i < stop; ++i) {
      dest[i] = src[i];
    }
  }

  void grow(size_t extend) {
    auto new_cap = bits::clp2(length + extend + GapSize);
    auto w = gap_width();
    T* tmp = mem::alloc<T>(sizeof(T) * new_cap, alignof(T));
    if (!tmp) return;

    uintptr_t ngb = gb - begin;
    uintptr_t nge = ngb + GapSize;

    shift(begin, gb, tmp);
    shift(ge, begin + length, tmp + nge);
  }

  T* begin;
  size_t length;
  size_t capacity;

  T* gb;
  T* ge;

  uintptr_t gap_width() { return ge - gb; }
};
}  // namespace ctr
