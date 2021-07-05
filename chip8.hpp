#pragma once

#include "buzzer.hpp"
#include "cpu.hpp"
#include "display.hpp"
#include "io.hpp"
#include "ram.hpp"
#include <istream>

class CHIP8
{
private:
  C8_CPU *cpu;
  C8_Display *display;
  C8_RAM *ram;
  C8_IO *io;
  C8_Buzzer *buzzer;

public:
  /*
   * Instantiate all composite components of the object.
   */
  CHIP8();

  /*
   * Destroy all the instantiated components.
   */
  ~CHIP8();

  /*
   * Loads and executes binary data from an input stream.
   *
   * @param is Input data stream that is loaded and executed.
   */
  void run_program(std::istream &is);
};
