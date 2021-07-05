#include "cpu.hpp"
#include <chrono>
#include <random>
#include <thread>

C8_CPU::C8_CPU() {}

C8_CPU::~C8_CPU() {}

void
CPU::reset()
{
  I = 0;
  program_counter = C8_PROGRAM_START;
  stack_pointer = C8_STACK_END - 2;
  sound_timer = 0;
  delay_timer = 0;
  cycles = 0;
  memset(registers, 0, 16);
}

uint16_t
byteswap(uint16_t u16)
{
  return (u16 >> 8) | (u16 << 8);
}

// In Hertz
#define CPU_CLOCK_SPEED 540
#define REFRESH_RATE    60

void
CPU::parse_instruction(
  C8_RAM &ram, C8_Display &display, C8_IO &io, C8_Buzzer &buzzer)
{
  if(cycles++ == CPU_CLOCK_SPEED / REFRESH_RATE) {
    cycles = 0;

    if(sound_timer > 0) {
      --sound_timer;
      buzzer.set_volume(1);
    } else {
      buzzer.set_volume(0);
    }

    delay_timer = delay_timer > 0 ? --delay_timer : 0;
    std::this_thread::sleep_for(
      std::chrono::milliseconds(1000 / REFRESH_RATE));
  }

  uint16_t instruction = 0;

  ram.read_memory(&instruction, program_counter, sizeof(instruction));
  program_counter += 2;
  instruction = byteswap(instruction);

  uint8_t &Vx = registers[(instruction >> 8) & 0xF];
  uint8_t &Vy = registers[(instruction >> 4) & 0xF];
  uint8_t &Vf = registers[15];
  uint8_t &V0 = registers[0];
  uint16_t nnn = (instruction & 0xFFF);
  uint8_t kk = (instruction & 0xFF);
  uint8_t n = (instruction & 0xF);

  switch((instruction >> 12) & 0xF) {
    case 0:
      switch(instruction) {
        case 0x00E0:
          display.clear_screen();
          return;
        case 0x00EE:
          stack_pointer += 2;
          ram.read_memory(
            &program_counter, stack_pointer, sizeof(program_counter));
          return;
        default:
          break;
      }
      break;

    case 1:
      program_counter = nnn;
      return;
    case 2:
      ram.write_memory(
        &program_counter, stack_pointer, sizeof(program_counter));
      stack_pointer -= 2;
      program_counter = nnn;
      return;
    case 3:
      if(Vx == kk)
        program_counter += 2;
      return;
    case 4:
      if(Vx != kk)
        program_counter += 2;
      return;
    case 5:
      if((instruction & 0xF) == 0 && Vx == Vy)
        program_counter += 2;
      return;
    case 6:
      Vx = kk;
      return;
    case 7:
      Vx += kk;
      return;
    case 8:
      switch(instruction & 0xF) {
        case 0:
          Vx = Vy;
          return;
        case 1:
          Vx |= Vy;
          return;
        case 2:
          Vx &= Vy;
          return;
        case 3:
          Vx ^= Vy;
          return;
        case 4:
          Vf = Vx + Vy > 255;
          Vx += Vy;
          return;
        case 5:
          Vf = Vx > Vy;
          Vx -= Vy;
          return;
        case 6:
          Vf = Vx & 1;
          Vx >>= 1;
          return;
        case 7:
          Vf = Vy > Vx;
          Vx = Vy - Vx;
          return;
        case 0xE:
          Vf = Vx & 0x8000;
          Vx <<= 1;
          return;
        default:
          break;
      }
      break;

    case 9:
      if(Vx != Vy)
        program_counter += 2;
      return;
    case 0xA:
      I = nnn;
      return;
    case 0xB:
      program_counter = nnn + V0;
      return;
    case 0xC: {
      std::random_device seeder;
      std::ranlux48 gen(seeder());
      std::uniform_int_distribution<uint8_t> uniform_byte(0, 255);
      Vx = kk & uniform_byte(gen);
    }
      return;
    case 0xD:
      uint8_t sprite[8 * 15];
      ram.read_memory(sprite, I, n);
      Vf = display.put_sprite(Vx, Vy, sprite, n);
      return;
    case 0xE: {
      switch(instruction & 0xFF) {
        case 0x9E:
          if(io.is_key_pressed(Vx))
            program_counter += 2;
          return;
        case 0xA1:
          if(!io.is_key_pressed(Vx))
            program_counter += 2;
          return;
        default:
          break;
      }
    } break;

    case 0xF:
      switch(instruction & 0xFF) {
        case 0x07:
          Vx = delay_timer;
          return;
        case 0x0A:
          Vx = io.wait_key_press();
          return;
        case 0x15:
          delay_timer = Vx;
          return;
        case 0x18:
          sound_timer = Vx;
          return;
        case 0x1E:
          I += Vx;
          return;
        case 0x29:
          I = C8_FONT_START + 5 * Vx;
          return;
        case 0x33:
          uint8_t arr[3];
          arr[0] = (Vx / 100) % 10;
          arr[1] = (Vx / 10) % 10;
          arr[2] = Vx % 10;
          ram.write_memory(arr, I, 3);
          break;
        case 0x55:
          ram.write_memory(registers, I, 1 + ((instruction >> 8) & 0xF));
          return;
        case 0x65:
          ram.read_memory(registers, I, 1 + ((instruction >> 8) & 0xF));
          return;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
