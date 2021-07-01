#include "io.hpp"
#include <stdexcept>

C8_IO::~C8_IO() {}

SDL_IO::SDL_IO() : user_quit(false)
{
  keybinds = {
    { 'x', 0x0 },
    { '1', 0x1 },
    { '2', 0x2 },
    { '3', 0x3 },
    { 'q', 0x4 },
    { 'w', 0x5 },
    { 'e', 0x6 },
    { 'a', 0x7 },
    { 's', 0x8 },
    { 'd', 0x9 },
    { 'z', 0xA },
    { 'c', 0xB },
    { '4', 0xC },
    { 'r', 0xD },
    { 'f', 0xE },
    { 'v', 0xF },
  };

  keypresses = { { 0x0, false },
    { 0x1, false },
    { 0x2, false },
    { 0x3, false },
    { 0x4, false },
    { 0x5, false },
    { 0x6, false },
    { 0x7, false },
    { 0x8, false },
    { 0x9, false },
    { 0xA, false },
    { 0xB, false },
    { 0xC, false },
    { 0xD, false },
    { 0xE, false },
    { 0xF, false } };
}

bool
SDL_IO::has_user_quit()
{
  return user_quit;
}

void
SDL_IO::handle_events()
{
  SDL_Event e;
  while(SDL_PollEvent(&e)) {
    switch(e.type) {
      case SDL_QUIT:
        user_quit = true;
        break;
      case SDL_KEYDOWN: {
        auto key = keybinds.find(e.key.keysym.sym);
        if(key != keybinds.end()) {
          keypresses[key->second] = true;
        }
      } break;
    }
  }
}

bool
SDL_IO::is_key_pressed(uint8_t key)
{
  bool is_pressed = keypresses[key];
  keypresses[key] = false;
  return is_pressed;
}

uint8_t
SDL_IO::wait_key_press()
{
  SDL_Event e;
  while(SDL_WaitEvent(&e)) {
    if(e.type == SDL_QUIT) {
      user_quit = true;
      return 0;
    } else if(e.type == SDL_KEYDOWN) {
      auto key = keybinds.find(e.key.keysym.sym);
      if(key != keybinds.end())
        return key->second;
    }
  }
  throw std::logic_error("Failed to grab a keypress");
}
