#include "kernel/tty/tty.hpp"

#include <cstddef>
#include <string_view>

#include "hal/keyboard.hpp"
#include "input/keymap.hpp"
#include "kernel/containers/string.hpp"

namespace tty {

void Tty::write_char(char c) noexcept {
  display.put_char(c);
  display.flush();
}

void Tty::write(std::string_view sv) noexcept {
  for (auto c : sv) {
    display.put_char(c);
  }
  display.flush();
}

void Tty::write(const ctr::String& s) noexcept {
  write(std::string_view{s.data(), s.size()});
}

void Tty::write_line(std::string_view sv) noexcept {
  write(sv);
  write_char('\n');
}

void Tty::write_line(ctr::String& s) noexcept {
  write_line(std::string_view{s.data(), s.size()});
}

char Tty::read_char_blocking() noexcept {
  for (;;) {
    hal::KeyEvent ev{};
    char c = 0;

    if (!keyboard.poll(ev)) continue;
    if (ev.type == hal::KeyEventType::Release) continue;
    if (!input::key_event_to_char(ev, c)) continue;

    return c;
  }
}

size_t Tty::readline(char* out, size_t capacity, std::string_view prompt) noexcept {
  if (!out || !capacity) return 0;

  size_t len = 0;
  write(prompt);

  for (;;) {
    hal::KeyEvent ev{};
    char c = 0;

    if (!keyboard.poll(ev)) continue;
    if (ev.type == hal::KeyEventType::Release) continue;

    if (ev.key == hal::Key::Backspace) {
      if (len > 0) {
        --len;
        display.backspace();
        display.flush();
      }
      continue;
    }

    if (!input::key_event_to_char(ev, c)) continue;

    if (c == '\n' || c == '\r') {
      out[len] = '\0';
      display.put_char('\n');
      display.flush();
      return len;
    }

    if (len + 1 < capacity) {
      out[len++] = c;
      display.put_char(c);
      display.flush();
    }
  }
}

void Tty::readline(ctr::String& out, std::string_view prompt) noexcept {
  out.clear();
  write(prompt);

  for (;;) {
    hal::KeyEvent ev{};
    char c = 0;

    if (!keyboard.poll(ev)) continue;
    if (ev.type == hal::KeyEventType::Release) continue;

    if (ev.key == hal::Key::Backspace) {
      if (!out.empty()) {
        out.pop_back();
        display.backspace();
        display.flush();
      }
      continue;
    }

    if (ev.key == hal::Key::PageUp) {
      display.scroll_up(1);
      display.flush();
      continue;
    }

    if (ev.key == hal::Key::PageDown) {
      display.scroll_down(1);
      display.flush();
      continue;
    }

    if (!input::key_event_to_char(ev, c)) continue;

    if (c == '\n' || c == '\r') {
      display.put_char('\n');
      display.flush();
      return;  // out.size();
    }

    out.push_back(c);
    display.put_char(c);
    display.flush();
  }
}

}  // namespace tty
