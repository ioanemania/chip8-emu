# chip8-emu
Emulator/Interpreter for the chip8 language written in c++

## Dependencies

Project uses `cmake` to generate build files and `sdl2` for graphics and handling events.

## How to use

After you build the binary:
`./chip8-emu <rom file>`

You can get roms from https://github.com/kripod/chip8-roms to play around.

## Sources used

- [Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [Wikipedia: CHIP-8](https://en.wikipedia.org/wiki/CHIP-8)

## Todo

- [ ] Implement a sound interface.
