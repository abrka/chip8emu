#pragma once
#include <cstdint>
#include <map>
#include <SDL.h>
#include <array>
#include <string>
#include <sstream>
#include <format>

static char const* chip8_file_extensions[2] = { "*.ch8", "*.c8" };


constexpr uint16_t program_starting_point{ 0x200 };
constexpr uint32_t size_of_mem{ 4096 };
constexpr uint8_t uninitialzed_value = 0xFF;
constexpr uint16_t stack_uninitialzed_value = 0xFFFF;

constexpr int bytes_displayed_per_tab{ 1000 };

constexpr float screen_scale = 12;
constexpr float screen_offset_x = 30;
constexpr float screen_offset_y = 8;

constexpr int chip8_screen_width = 64;
constexpr int chip8_screen_height = 32;

constexpr uint8_t bytes_read_per_opcode{ 2 };
constexpr uint8_t sprite_width{ 8 };

using chip8_color = bool;
constexpr bool chip8_color_lit = true;
constexpr bool chip8_color_unlit = false;

struct rgba_color {
	uint8_t r, g, b, a;
};
[[maybe_unused]] constexpr rgba_color neon = { 57, 255, 20, 255 };
[[maybe_unused]] constexpr rgba_color black = { 0,0,0,255 };
[[maybe_unused]] constexpr rgba_color white = { 255,255,255,255 };

static rgba_color chip8_color_to_rgba(chip8_color col) {
	if (col == true) {
		return neon;
	}
	else {
		return black;
	}
}

constexpr uint16_t font_data_starting_point = 0;
constexpr uint8_t size_of_font = 5;

constexpr std::array<uint8_t, 5> chip8_font_0 = { 0xF0, 0x90 ,0x90, 0x90 , 0xF0 };
constexpr std::array<uint8_t, 5> chip8_font_1 = { 0x20 ,0x60 ,0x20 ,0x20 , 0x70 };
constexpr std::array<uint8_t, 5> chip8_font_2 = { 0xF0 ,0x10, 0xF0 ,0x80, 0xF0 };
constexpr std::array<uint8_t, 5> chip8_font_3 = { 0xF0 ,0x10, 0xF0 , 0x10 ,0xF0 };
constexpr std::array<uint8_t, 5> chip8_font_4 = { 0x90, 0x90 ,0xF0,0x10, 0x10 };
constexpr std::array<uint8_t, 5> chip8_font_5 = { 0xF0, 0x80, 0xF0, 0x10, 0xF0 };
constexpr std::array<uint8_t, 5> chip8_font_6 = { 0xF0, 0x90 ,0x90, 0x90 , 0xF0 };
constexpr std::array<uint8_t, 5> chip8_font_7 = { 0xF0, 0x90 ,0x90, 0x90 , 0xF0 };
constexpr std::array<uint8_t, 5> chip8_font_8 = { 0xF0, 0x90 ,0x90, 0x90 , 0xF0 };
constexpr std::array<uint8_t, 5> chip8_font_9 = { 0xF0, 0x90 ,0xF0 ,0x10, 0xF0 };
constexpr std::array<uint8_t, 5> chip8_font_A = { 0xF0, 0x90 ,0x90, 0x90 , 0xF0 };
constexpr std::array<uint8_t, 5> chip8_font_B = { 0xF0, 0x90 ,0x90, 0x90 , 0xF0 };
constexpr std::array<uint8_t, 5> chip8_font_C = { 0xF0, 0x90 ,0x90, 0x90 , 0xF0 };
constexpr std::array<uint8_t, 5> chip8_font_D = { 0xF0, 0x90 ,0x90, 0x90 , 0xF0 };
constexpr std::array<uint8_t, 5> chip8_font_E = { 0xF0, 0x90 ,0x90, 0x90 , 0xF0 };
constexpr std::array<uint8_t, 5> chip8_font_F = { 0xF0, 0x90 ,0x90, 0x90 , 0xF0 };

constexpr std::array chip8_fonts = {
	chip8_font_0,
	chip8_font_1, 
	chip8_font_2, 
	chip8_font_3,
	chip8_font_4,
	chip8_font_5,
	chip8_font_6,
	chip8_font_7,
	chip8_font_8, 
	chip8_font_9,
	chip8_font_A, 
	chip8_font_B,
	chip8_font_C,
	chip8_font_D,
	chip8_font_E,
	chip8_font_F };


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

static std::string byte_to_hex_str(uint8_t byte) {
	return std::format("0x{:02x}  ", byte);
}

static std::string byte_to_hex_str_ns(uint8_t byte) {
		return std::format("0x{:02x}", byte);
}

static std::string word_to_hex_str(uint16_t word) {
	return std::format("0x{:04x}  ", word);
}