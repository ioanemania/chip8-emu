#include "buzzer.hpp"
#include <cmath>

C8_Buzzer::~C8_Buzzer() {}

SDL_Buzzer::SDL_Buzzer() : volume(0)
{
  SDL_Init(SDL_INIT_AUDIO);
  SDL_zero(spec_desired);
  spec_desired.freq = BUZZER_FREQUENCY;
  spec_desired.format = AUDIO_S16SYS;
  spec_desired.channels = 1;
  spec_desired.samples = BUZZER_FREQUENCY / 60;
  spec_desired.userdata = this;
  spec_desired.callback = [](void *_userdata, uint8_t *_stream, int _len) {
    ((SDL_Buzzer *) _userdata)->generate_square_wave(nullptr, _stream, _len);
  };
  SDL_OpenAudio(&spec_desired, &spec_obtained);
  SDL_PauseAudio(0);
}

SDL_Buzzer::~SDL_Buzzer()
{
  SDL_CloseAudio();
  SDL_AudioQuit();
}

void
SDL_Buzzer::set_volume(bool on)
{
  SDL_LockAudio();
  volume = on;
  SDL_UnlockAudio();
}

double
SDL_Buzzer::square_wave(double angle)
{
  return (std::sin(angle) < 0.0 ? -1.0 : 1.0) * BUZZER_AMPLITUDE;
}

void
SDL_Buzzer::generate_square_wave(void *_userdata, uint8_t *_stream, int _len)
{
  double angle = 0;
  double angle_rate = (2.0 * M_PI / BUZZER_FREQUENCY) * 440;
  for(int i = 0; i < _len / 2; i++) {
    ((uint16_t *) _stream)[i] = square_wave(angle) * volume;
    angle += angle_rate;
  }
}
