#pragma once

#include <SDL.h>
#include <cstdint>

#define BUZZER_AMPLITUDE 28000
#define BUZZER_FREQUENCY 44100

class C8_Buzzer
{
public:
  virtual ~C8_Buzzer();

  /*
   * Enables or disables the buzzer sound
   */
  virtual void set_volume(bool on) = 0;
};

class SDL_Buzzer : public C8_Buzzer
{
private:
  SDL_AudioSpec spec_desired;
  SDL_AudioSpec spec_obtained;
  bool volume;

public:
  SDL_Buzzer();
  ~SDL_Buzzer();
  virtual void set_volume(bool on) override;

private:
  double square_wave(double angle);
  void generate_square_wave(void *_userdata, uint8_t *_stream, int _len);
};
