#pragma once

#include <SDL.h>

#define C8_DISPLAY_WIDTH  64
#define C8_DISPLAY_HEIGHT 32

class C8_Display
{
public:
  virtual ~C8_Display();

  /*
   * Fill the display buffer with black pixels.
   */
  virtual void clear_screen() = 0;

  /*
   * Render modified display buffer to the screen.
   */
  virtual void update_screen() = 0;

  /*
   * Copy sprite data to the display buffer.
   *
   * The sprite is copied by XORing every byte with one in the display buffer.
   *
   * @param x Starting x axis coordinate.
   * @param y Starting y axis coordinate.
   * @param sprite Pointer to the byte array that is copied to the display
   * buffer.
   * @param n Number of bytes that are copied.
   * @return true if XORing one of the bytes caused a white pixel to become
   * black, otherwise false (Collision).
   */
  virtual bool put_sprite(int x, int y, uint8_t *sprite, uint8_t n) = 0;
};

class SDLDisplay : public C8_Display
{
private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  uint32_t pixels[C8_DISPLAY_WIDTH * C8_DISPLAY_HEIGHT];

public:
  SDLDisplay();
  ~SDLDisplay();
  virtual void clear_screen() override;
  virtual void update_screen() override;
  virtual bool put_sprite(int x, int y, uint8_t *sprite, uint8_t n) override;
};
