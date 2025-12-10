#pragma once

#include "drv/core.hpp"

namespace drv {

using GlobalCore = DriverCore<32, 16, 32>;

GlobalCore& core() noexcept;
}  // namespace drv
