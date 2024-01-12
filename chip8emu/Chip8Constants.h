#pragma once
#include <cstdint>
#include <map>
#include <SDL.h>

constexpr uint16_t program_starting_point{ 0x200 };
constexpr uint8_t uninitialzed_value = 0xFF;

constexpr int chip8_screen_width = 64;
constexpr int chip8_screen_height = 32;

using chip8_color = bool;
constexpr bool chip8_color_lit = true;
constexpr bool chip8_color_unlit = false;

using chip8_keycode = uint8_t;
static std::map<SDL_KeyCode, chip8_keycode> SDL_input_to_chip8_input_map{
	{SDLK_KP_0, 0x1},	{SDLK_KP_0,0x2 }, {SDLK_KP_0,0x3}, {SDLK_KP_0,0xC},
	{SDLK_KP_0,0x4}, {SDLK_KP_0,0x5}, {SDLK_KP_0,0x6},{ SDLK_KP_0,0xD},
	{SDLK_KP_0,0x7}, {SDLK_KP_0,0x8}, {SDLK_KP_0,0x9},{ SDLK_KP_0,0xE},
	{SDLK_KP_0,0xA}, {SDLK_KP_0,0x0}, {SDLK_KP_0,0xB}, {SDLK_KP_0,0xF},
};

static chip8_keycode SDL_input_to_chip8_input(SDL_KeyCode key) {
	return SDL_input_to_chip8_input_map[key];
};