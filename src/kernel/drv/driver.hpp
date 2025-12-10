#pragma once

#include "drv/type_tag.hpp"

namespace drv {

class DriverBase {
 public:
  virtual ~DriverBase() = default;
  virtual TypeTag iface_type() const noexcept = 0;
  virtual TypeTag desc_type() const noexcept = 0;
  virtual bool match_generic(const void* desc) const noexcept = 0;
  virtual void* probe_generic(const void* desc) noexcept = 0;
};

template <typename Iface, typename Desc>
class Driver : public DriverBase {
 public:
  static TypeTag iface_tag() noexcept { return TypeTag::of<Iface>(); }

  static TypeTag desc_tag() noexcept { return TypeTag::of<Desc>(); }

  TypeTag iface_type() const noexcept override { return iface_tag(); }

  TypeTag desc_type() const noexcept override { return desc_tag(); }

  bool match_generic(const void* desc) const noexcept override {
    return match(*static_cast<const Desc*>(desc));
  }

  void* probe_generic(const void* desc) noexcept override {
    return probe(*static_cast<const Desc*>(desc));
  }

  virtual bool match(const Desc& desc) const noexcept = 0;
  virtual Iface* probe(const Desc& desc) noexcept = 0;
};

}  // namespace drv
