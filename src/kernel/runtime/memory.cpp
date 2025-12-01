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

namespace std {

void* memset(void* dest, int ch, size_t count) {
  return ::memset(dest, ch, count);
}

}  // namespace std
