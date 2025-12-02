#include <cstddef>
#include <cstdint>

extern "C" void* memset(void* dest, int ch, size_t count) noexcept {
  auto* p = static_cast<uint8_t*>(dest);
  auto v = static_cast<uint8_t>(ch);

  for (size_t i = 0; i < count; ++i) {
    p[i] = v;
  }

  return dest;
}

extern "C" void* memcpy(void* __restrict__ dest, const void* __restrict__ src,
                        size_t count) {
  if (reinterpret_cast<uintptr_t>(dest) % sizeof(size_t) == 0 &&
      reinterpret_cast<uintptr_t>(src) % sizeof(size_t) == 0 &&
      count % sizeof(size_t) == 0) {
    size_t* d = reinterpret_cast<size_t*>(dest);
    const size_t* s = reinterpret_cast<const size_t*>(src);

    for (size_t i = 0; i < count / sizeof(size_t); ++i) {
      d[i] = s[i];
    }
  } else {
    char* d = reinterpret_cast<char*>(dest);
    const char* s = reinterpret_cast<char*>(dest);
    for (size_t i = 0; i < count; ++i) {
      d[i] = s[i];
    }
  }

  return dest;
}

namespace std {

void* memset(void* dest, int ch, size_t count) {
  return ::memset(dest, ch, count);
}

void* memcpy(void* dest, const void* src, size_t count) {
  return ::memcpy(dest, src, count);
}

}  // namespace std
