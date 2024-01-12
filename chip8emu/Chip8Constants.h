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
static std::map<SDL_Keycode, chip8_keycode> SDL_input_to_chip8_input_map{
	{SDLK_KP_7,0x1}, {SDLK_KP_8,0x2}, {SDLK_KP_9,0x3}, {SDLK_KP_PLUS,0xC},
	{SDLK_KP_4,0x4}, {SDLK_KP_5,0x5}, {SDLK_KP_6,0x6}, {SDLK_KP_PLUS,0xD},
	{SDLK_KP_1,0x7}, {SDLK_KP_2,0x8}, {SDLK_KP_3,0x9}, {SDLK_KP_ENTER,0xE},
	{SDLK_KP_0,0xA}, {SDLK_KP_0,0x0}, {SDLK_KP_PERIOD,0xB}, {SDLK_KP_ENTER,0xF},
};

static std::optional<chip8_keycode> SDL_input_to_chip8_input(SDL_Keycode key) {
	if (SDL_input_to_chip8_input_map.contains(key)) {
		return SDL_input_to_chip8_input_map[key];
	}
	else {
		return {};
	}
};