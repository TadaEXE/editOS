#pragma once

#include <cstdint>

namespace hal {

using EntryFn = void (*)(void*);

class CpuContext {
 public:
  virtual ~CpuContext() = default;
};

class Cpu {
 public:
  virtual ~Cpu() = default;

  virtual bool create_context(CpuContext& context, EntryFn entry, void* arg,
                              uintptr_t stack_top) const noexcept = 0;

  virtual void switch_to(const CpuContext& next) noexcept = 0;

  virtual const CpuContext& current_context() noexcept = 0;
};

}  // namespace hal
