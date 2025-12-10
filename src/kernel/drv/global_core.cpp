#include "drv/global_core.hpp"

namespace drv {
static GlobalCore g_core;

GlobalCore& core() noexcept {
  return g_core;
}

}  // namespace drv
