cmake_minimum_required(VERSION 3.22)
project(chip8emu_proj)



file(GLOB CHIP8EMU_SOURCES src/chip8emu/*.cpp src/chip8emu/*.h) 
add_executable(chip8emu ${CHIP8EMU_SOURCES})
set_target_properties(chip8emu PROPERTIES CXX_STANDARD 20)

target_include_directories(chip8emu PUBLIC external)

file(GLOB IMGUI_SOURCES external/imgui-docking/*.cpp  external/imgui-docking/*.h external/imgui-docking/misc/cpp/imgui_stdlib.*)
list(APPEND IMGUI_SOURCES "external/imgui-docking/backends/imgui_impl_sdl2.cpp" "external/imgui-docking/backends/imgui_impl_sdl2.h" "external/imgui-docking/backends/imgui_impl_sdl2.h" "external/imgui-docking/backends/imgui_impl_sdlrenderer2.cpp" "external/imgui-docking/backends/imgui_impl_sdlrenderer2.h")
add_library(imguilib STATIC ${IMGUI_SOURCES})
target_include_directories(imguilib PUBLIC external/imgui-docking external/imgui-docking/backends)
target_include_directories(imguilib PUBLIC external/SDL/include)

target_link_libraries(chip8emu imguilib)


add_subdirectory(external/SDL)
target_link_libraries(chip8emu SDL2main SDL2-static)


add_library(tinyfiledialogs STATIC external/tinyfiledialogs/tinyfiledialogs.c external/tinyfiledialogs/tinyfiledialogs.h)
target_link_libraries(chip8emu tinyfiledialogs)

