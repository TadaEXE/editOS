#pragma once

#include <cstddef>

namespace drv {

struct TypeTag {
  const void* token;

  template <typename T>
  static TypeTag of() noexcept {
    static const size_t unique{0};
    return TypeTag{&unique};
  }

  bool operator==(TypeTag other) const noexcept { return token == other.token; }

  bool operator!=(TypeTag other) const noexcept { return token != other.token; }
};

}  // namespace drv
