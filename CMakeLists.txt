cmake_minimum_required(VERSION 3.20)

project(chip8-emu VERSION 0.1 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(SDL2 REQUIRED)

add_executable(chip8-emu 
  main.cpp 
  chip8.cpp
  chip8.hpp
  cpu.cpp
  cpu.hpp
  ram.hpp
  ram.cpp
  display.cpp
  display.hpp
  io.hpp
  io.cpp
  buzzer.cpp
  buzzer.hpp
)

target_link_libraries(chip8-emu SDL2::SDL2)
target_include_directories(chip8-emu PUBLIC ${SDL2_INCLUDE_DIRS})
