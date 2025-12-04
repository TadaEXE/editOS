#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "kernel/memory/heap.hpp"
#include "kernel/panic.hpp"
#include "math/bit_logic.hpp"

namespace ctr {

namespace {
template <typename T>
constexpr size_t min_gap_size() {
  return bits::flp2(bits::oiz(32 / sizeof(T)));
}
}  // namespace

template <typename T, size_t GapSize = min_gap_size<T>()>
class GapBuffer {
 public:
  explicit GapBuffer(size_t initial_capacity = GapSize * 2)
      : length(0),
        capacity(initial_capacity < GapSize ? GapSize * 2 : initial_capacity),
        begin(mem::alloc<T>(capacity, alignof(T))),
        gap_begin(begin),
        gap_end(gap_begin + GapSize) {
    if (!begin) panic("Allocation of GapBuffer failed");
  }

  ~GapBuffer() { mem::free(begin); }

  const T& operator[](size_t idx) const {
    if (idx >= length) {
      panic("Out of range access of gap buffer at %u with length %u.", idx, length);
    }

    size_t prefix_len = gap_begin - begin;
    if (idx < prefix_len) {
      return begin[idx];
    } else {
      return gap_end[idx - prefix_len];
    }
  }

  void insert(const T* items, size_t count = 1) {
    if (!items) return;
    if (!count) return;

    if (gap_width() >= count) {
      for (size_t i = 0; i < count; ++i) {
        *gap_begin++ = items[i];
      }
      length += count;
      return;
    }

    grow(count);
    insert(items, count);
  }

  void move_left(size_t count = 1) {
    if (!count) return;

    size_t prefix_len = gap_begin - begin;
    if (count > prefix_len) { count = prefix_len; }

    memmove(gap_end - count, gap_begin - count, count * sizeof(T));
    gap_begin -= count;
    gap_end -= count;
  }

  void delete_left(uintptr_t count = 1) {
    if (!count) return;

    auto prev_gap_begin = gap_begin;
    gap_begin = gap_begin - count > begin ? gap_begin - count : begin;
    length -= static_cast<size_t>(prev_gap_begin - gap_begin);
  }

  void move_right(uintptr_t count = 1) {
    if (!count) return;

    auto suffix_len = length - (gap_end - begin);
    if (count > suffix_len) { count = suffix_len; }

    if ((gap_end + count) <= begin + length) {
      memmove(gap_begin, gap_end, count * sizeof(T));
      gap_begin += count;
      gap_end += count;
    }
  }

  void delete_right(uintptr_t count = 1) {
    if (!count) return;

    auto prev_gap_end = gap_end;
    gap_end = gap_end + count < begin + length ? gap_end + count : begin + length;
    length -= static_cast<size_t>(gap_end - prev_gap_end);
  }

  void move_to(uintptr_t index) {
    auto cp = cursor_pos();
    if (index < cp) {
      move_left(cp - index);
    } else if (index > cp) {
      move_right(index - cp);
    }
  }

  size_t cursor_pos() { return gap_begin - begin; }

  size_t count() { return length; }

 private:
  void grow(size_t extend) {
    if (!extend) return;

    auto new_cap = bits::clp2(length + extend + GapSize);

    size_t new_gap_begin_offset = gap_begin - begin;
    size_t new_gap_end_offset =
        new_gap_begin_offset + (extend < GapSize ? GapSize : extend);
    size_t prefix_len = gap_begin - begin;
    size_t suffix_len = length - prefix_len;

    log_msg("grow(%u) %u -> %u (%u; %u)", extend, capacity, new_cap, prefix_len, suffix_len);

    if (new_cap <= capacity) {
      memmove(begin + new_gap_end_offset, gap_end, suffix_len * sizeof(T));
      gap_end = begin + new_gap_end_offset;
      return;
    }

    if (T* tmp = mem::alloc<T>(new_cap, alignof(T))) {

      memmove(tmp, begin, prefix_len * sizeof(T));
      memmove(tmp + new_gap_end_offset, gap_end, suffix_len * sizeof(T));

      mem::free(begin);

      capacity = new_cap;
      begin = tmp;
      gap_begin = begin + new_gap_begin_offset;
      gap_end = begin + new_gap_end_offset;
      return;
    }

    panic("Malloc failed");
  }

  size_t length;
  size_t capacity;

  T* begin;
  T* gap_begin;
  T* gap_end;

  size_t gap_width() { return gap_end - gap_begin; }
};
}  // namespace ctr
