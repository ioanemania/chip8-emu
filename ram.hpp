#pragma once

#include <cstdint>
#include <istream>

#define C8_MEMORY_SIZE  4096
#define C8_FONT_SIZE    512
#define C8_STACK_SIZE   48
#define C8_PROGRAM_SIZE C8_MEMORY_SIZE - (C8_FONT_SIZE + C8_STACK_SIZE)

#define C8_FONT_START    0x0
#define C8_PROGRAM_START C8_FONT_START + C8_FONT_SIZE
#define C8_STACK_START   C8_PROGRAM_START + C8_PROGRAM_SIZE
#define C8_STACK_END     C8_STACK_START + C8_STACK_SIZE

class C8_RAM
{
public:
  virtual ~C8_RAM();

  /*
   * Copy a program from an input stream to the internal memory buffer.
   *
   * @param is Input stream which the data is copied from.
   */
  virtual void load_program_data(std::istream &is) = 0;

  /*
   * Copy data from the internal memory buffer.
   *
   * @param dest Pointer to the location the data is copied to.
   * @param addr Start address in the internal memory buffer where the
   * data is copied from.
   * @param n Number of bytes to copy.
   */
  virtual void read_memory(void *dest, uint16_t addr, uint32_t n = 1) = 0;

  /*
   * Copy data to the internal memory buffer.
   *
   * @param src Pointer to the location where the data is copied from.
   * @param addr Start address in the internal memory buffer where the data is
   * copied to.
   * @param n Number of bytes to copy.
   */
  virtual void write_memory(void *src, uint16_t addr, uint32_t n = 1) = 0;
};

class RAM : public C8_RAM
{
private:
  uint8_t font_data[512];
  uint8_t program_data[C8_PROGRAM_SIZE];
  uint8_t stack[C8_STACK_SIZE];

public:
  RAM();
  virtual void load_program_data(std::istream &is) override;
  virtual void read_memory(void *dest, uint16_t addr, uint32_t n = 1) override;
  virtual void write_memory(void *src, uint16_t addr, uint32_t n = 1) override;
};
