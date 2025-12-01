#include "hal/keyboard.hpp"

namespace hal {
KeyMod modifier_for_key(Key key) noexcept {
  switch (key) {
    case Key::LeftShift:
      [[fallthrough]];
    case Key::RightShift:
      return KeyMod::Shift;

    case Key::LeftCtrl:
      [[fallthrough]];
    case Key::RightCtrl:
      return KeyMod::Ctrl;

    case Key::LeftAlt:
      return KeyMod::Alt;

    case Key::RightAlt:
      return KeyMod::AltGr;

    case Key::LeftGui:
      [[fallthrough]];
    case Key::RightGui:
      return KeyMod::Super;

    case Key::CapsLock:
      return KeyMod::CapsLock;

    case Key::NumLock:
      return KeyMod::NumLock;

    case Key::ScrollLock:
      return KeyMod::ScrollLock;

    default:
      return KeyMod::None;
  }
}

bool is_lock_key(Key key) noexcept {
  return key == Key::CapsLock || key == Key::NumLock || key == Key::ScrollLock;
}

}  // namespace hal
