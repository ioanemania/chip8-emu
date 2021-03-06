#include "ram.hpp"
#include <cstring>

C8_RAM::~C8_RAM() {}

RAM::RAM()
{
  struct font_char {
    uint8_t bytes[5];
  };

  ((font_char *) font_data)[0x0] = { 0xF0, 0x90, 0x90, 0x90, 0xF0 };
  ((font_char *) font_data)[0x1] = { 0x20, 0x60, 0x20, 0x20, 0x70 };
  ((font_char *) font_data)[0x2] = { 0xF0, 0x10, 0xF0, 0x80, 0xF0 };
  ((font_char *) font_data)[0x3] = { 0xF0, 0x10, 0xF0, 0x10, 0xF0 };
  ((font_char *) font_data)[0x4] = { 0x90, 0x90, 0xF0, 0x10, 0x10 };
  ((font_char *) font_data)[0x5] = { 0xF0, 0x80, 0xF0, 0x10, 0xF0 };
  ((font_char *) font_data)[0x6] = { 0xF0, 0x80, 0xF0, 0x90, 0xF0 };
  ((font_char *) font_data)[0x7] = { 0xF0, 0x10, 0x20, 0x40, 0x40 };
  ((font_char *) font_data)[0x8] = { 0xF0, 0x90, 0xF0, 0x90, 0xF0 };
  ((font_char *) font_data)[0x9] = { 0xF0, 0x90, 0xF0, 0x10, 0xF0 };
  ((font_char *) font_data)[0xA] = { 0xF0, 0x90, 0xF0, 0x90, 0x90 };
  ((font_char *) font_data)[0xB] = { 0xE0, 0x90, 0xE0, 0x90, 0xE0 };
  ((font_char *) font_data)[0xC] = { 0xF0, 0x80, 0x80, 0x80, 0xF0 };
  ((font_char *) font_data)[0xD] = { 0xE0, 0x90, 0x90, 0x90, 0xE0 };
  ((font_char *) font_data)[0xE] = { 0xF0, 0x80, 0xF0, 0x80, 0xF0 };
  ((font_char *) font_data)[0xF] = { 0xF0, 0x80, 0xF0, 0x80, 0x80 };
}

void
RAM::load_program_data(std::istream &is)
{
  is.read((char *) program_data, C8_PROGRAM_SIZE);
}

void
RAM::read_memory(void *dest, uint16_t addr, uint32_t n)
{
  uint8_t *data = nullptr;
  if(addr < C8_PROGRAM_START)
    data = font_data;
  else if(addr < C8_STACK_START) {
    data = program_data;
    addr -= C8_PROGRAM_START;
  } else if(addr < C8_STACK_END) {
    data = stack;
    addr -= C8_STACK_START;
  } else
    throw std::runtime_error("[ ERROR ] Tried to read inaccessible memory.");

  memcpy(dest, data + addr, n);
}

void
RAM::write_memory(void *src, uint16_t addr, uint32_t n)
{
  uint8_t *data = nullptr;
  if(addr >= C8_PROGRAM_START && addr < C8_STACK_START) {
    data = program_data;
    addr -= C8_PROGRAM_START;
  } else if(addr >= C8_STACK_START && addr < C8_STACK_END) {
    data = stack;
    addr -= C8_STACK_START;
  } else
    throw std::runtime_error("[ ERROR ] Tried to write protected memory.");

  memcpy(data + addr, src, n);
}
