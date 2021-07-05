#include "chip8.hpp"

CHIP8::CHIP8()
    : cpu(new CPU), display(new SDLDisplay), io(new SDL_IO), ram(new RAM),
      buzzer(new SDL_Buzzer)
{
}

CHIP8::~CHIP8()
{
  delete cpu;
  delete ram;
  delete io;
  delete buzzer;
  delete display;
}

void
CHIP8::run_program(std::istream &is)
{
  ram->load_program_data(is);
  cpu->reset();
  while(!io->has_user_quit()) {
    cpu->parse_instruction(*ram, *display, *io, *buzzer);
    io->handle_events();
    display->update_screen();
  }
}
