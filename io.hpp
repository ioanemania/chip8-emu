#pragma once
#include <SDL.h>
#include <unordered_map>

class C8_IO
{
public:
  virtual ~C8_IO();
  virtual bool has_user_quit() = 0;

  /*
   * Handles all incoming events related to the keyboard or user interface.
   */
  virtual void handle_events() = 0;

  /*
   * Check if a key in the range of 0x0 - 0xF has been pressed in previous
   * cycles. If true reset the keypressed state to false.
   *
   * @param key Key that is checked.
   * @return true if the key was pressed, otherwise false.
   */
  virtual bool is_key_pressed(uint8_t key) = 0;

  /*
   * Halt executing and wait for a keypress.
   *
   * @return corresponding hexadecimal value for the pressed key, in the range
   * of 0x0 - 0xF.
   */
  virtual uint8_t wait_key_press() = 0;
};

class SDL_IO : public C8_IO
{
private:
  struct KEY_PAIR {
    uint8_t key;
    bool is_pressed;
  };

  std::unordered_map<uint8_t, bool> keypresses;
  std::unordered_map<char, uint8_t> keybinds;
  bool user_quit;

public:
  SDL_IO();
  virtual bool has_user_quit() override;
  virtual void handle_events() override;
  virtual bool is_key_pressed(uint8_t key) override;
  virtual uint8_t wait_key_press() override;
};
