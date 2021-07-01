#include "display.hpp"

#define PIXEL_SIZE        10
#define SDL_SCREEN_WIDTH  C8_DISPLAY_WIDTH *PIXEL_SIZE
#define SDL_SCREEN_HEIGHT C8_DISPLAY_HEIGHT *PIXEL_SIZE

C8_Display::~C8_Display() {}

SDLDisplay::SDLDisplay()
{
  SDL_Init(SDL_INIT_VIDEO);
  SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
  window = SDL_CreateWindow("CHIP8",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    SDL_SCREEN_WIDTH,
    SDL_SCREEN_HEIGHT,
    0);
  renderer = SDL_CreateRenderer(window, -1, 0);
  texture = SDL_CreateTexture(renderer,
    SDL_PIXELFORMAT_RGB888,
    SDL_TEXTUREACCESS_TARGET,
    C8_DISPLAY_WIDTH,
    C8_DISPLAY_HEIGHT);
}

SDLDisplay::~SDLDisplay()
{
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void
SDLDisplay::clear_screen()
{
  SDL_RenderClear(renderer);
  memset(pixels, 0, sizeof(pixels));
}

void
SDLDisplay::update_screen()
{
  SDL_RenderCopy(renderer, texture, 0, 0);
  SDL_RenderPresent(renderer);
}

bool
SDLDisplay::put_sprite(int x, int y, uint8_t *sprite, uint8_t n)
{
  bool collision = false;
  const int pitch
    = 256; // each pixel 4 bytes, 64 pixels width -> 256 bytes per row
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < 8; j++) {
      uint32_t location = (i + y) * pitch / 4 + (x + j);
      if(location < sizeof(pixels) / 4) { // Make sure we are not out of bounds
        // 7 - j instead of j due to different endianness
        pixels[location] ^= ((sprite[i] >> (7 - j)) & 0x1) * 0xFFFFFF;
        if(!collision)
          collision = (!pixels[location]) && ((sprite[i] >> (7 - j)) & 0x1);
      }
    }
  }
  SDL_UpdateTexture(texture, NULL, pixels, pitch);
  return collision;
}
