#include "chip8.hpp"
#include <fstream>
#include <iostream>

int
main(int argc, char **argv)
{
  if(argc != 2) {
    std::cout << "usage: ./chip8-emu <filename>";
    return 1;
  }

  CHIP8 chip8;
  std::fstream fs(argv[1], std::fstream::in);
  chip8.run_program(fs);
}
