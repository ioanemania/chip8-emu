#pragma once

#include "display.hpp"
#include "io.hpp"
#include "ram.hpp"

class C8_CPU
{
public:
  C8_CPU();
  virtual ~C8_CPU();

  /*
   * Resets all internal registers and timers to their corresponding initial
   * values.
   */
  virtual void reset() = 0;

  /*
   * Reads an instruction from memory and executes it.
   *
   * @param ram C8_RAM instance that an instruction is read from.
   * @param display C8_DISPLAY graphics instance that is modified in case of a
   * related instruction.
   * @param io C8_IO input/output event instance that is modified in case of a
   * related instruction.
   */
  virtual void parse_instruction(C8_RAM &ram, C8_Display &display, C8_IO &io)
    = 0;
};

class CPU : public C8_CPU
{
private:
  uint16_t I;
  uint8_t registers[16];
  uint16_t program_counter;
  uint16_t stack_pointer;
  uint32_t cycles;
  uint16_t sound_timer;
  uint16_t delay_timer;

public:
  virtual void reset() override;
  virtual void parse_instruction(
    C8_RAM &ram, C8_Display &display, C8_IO &io) override;
};
