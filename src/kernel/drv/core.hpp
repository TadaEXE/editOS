#pragma once
#include <cstddef>

#include "drv/driver.hpp"
#include "drv/type_tag.hpp"

namespace drv {

// TODO: Make this dynamic
template <size_t MaxDescs, size_t MaxDrivers, size_t MaxBindings>
class DriverCore {
 public:
  struct DescEntry {
    TypeTag type;
    const void* ptr;
  };

  struct Binding {
    TypeTag iface_type;
    const DescEntry* desc;
    void* iface;
    DriverBase* driver;
  };

  DriverCore() = default;

  template <typename Desc>
  void add_desc(const Desc& desc) noexcept {
    if (desc_count >= MaxDescs) { return; }
    descs[desc_count++] = DescEntry{
        TypeTag::of<Desc>(),
        &desc,
    };
  }

  void register_driver(DriverBase& drv) noexcept {
    if (driver_count >= MaxDrivers) { return; }
    drivers[driver_count++] = &drv;
  }

  void bind_all() noexcept {
    for (size_t di = 0; di < desc_count; ++di) {
      DescEntry& de = descs[di];

      for (size_t dr = 0; dr < driver_count; ++dr) {
        DriverBase* drv = drivers[dr];

        if (drv->desc_type() != de.type) { continue; }
        if (!drv->match_generic(de.ptr)) { continue; }

        void* iface_ptr = drv->probe_generic(de.ptr);
        if (!iface_ptr) { continue; }

        if (binding_count >= MaxBindings) { break; }

        bindings[binding_count++] = Binding{
            drv->iface_type(),
            &de,
            iface_ptr,
            drv,
        };
      }
    }
  }

  template <typename Iface>
  Iface* first_of_type() noexcept {
    TypeTag tag = TypeTag::of<Iface>();
    for (size_t i = 0; i < binding_count; ++i) {
      if (bindings[i].iface_type == tag) {
        return static_cast<Iface*>(bindings[i].iface);
      }
    }

    return nullptr;
  }

  template <typename Iface, typename Pred>
  Iface* find_if(Pred pred) noexcept {
    TypeTag tag = TypeTag::of<Iface>();
    for (size_t i = 0; i < binding_count; ++i) {
      Binding& b = bindings[i];

      if (b.iface_type != tag) { continue; }
      if (!pred(*b.desc, b.iface, b.driver)) { continue; }
      return static_cast<Iface*>(b.iface);
    }

    return nullptr;
  }

  const Binding* get_bindings() const noexcept { return bindings; }

  size_t get_bindings_count() const noexcept { return binding_count; }

 private:
  DescEntry descs[MaxDescs]{};
  DriverBase* drivers[MaxDrivers]{};
  Binding bindings[MaxBindings];

  size_t desc_count{0};
  size_t driver_count{0};
  size_t binding_count{0};
};

}  // namespace drv
