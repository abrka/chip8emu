#pragma once
#include <cstdint>
#include <array>
#include <assert.h>
#include <functional>
#include <iostream>
#include <fstream>
#include <optional>
#include "Chip8Constants.h"
#include <SDL.h>

struct Bus {

	std::array<uint8_t, 0xFFFF> memory{};
	std::array<uint16_t, 64>stack{};
	std::array<std::array<chip8_color, chip8_screen_width>, chip8_screen_height> pixels{};
	std::optional<chip8_keycode> pressed_key{};

	//only set when code is loaded from file
	std::optional<uint16_t> program_code_size = {};

	Bus() {
		reset();
		load_fonts_into_mem();
	}

	uint16_t get_adress_of_font(uint8_t which_font) const {
		return font_data_starting_point + (which_font * size_of_font);
	}
	void load_fonts_into_mem() {
		int font_data_index = font_data_starting_point;

		for (const auto& font : chip8_fonts)
		{
			for (const uint8_t& font_data : font) {
				memory[font_data_index] = font_data;
				font_data_index++;
			}
		}
	};

	void reset() {
		memory.fill(uninitialzed_value);
		stack.fill(uninitialzed_value);
		pressed_key = {};
		for (auto& column : pixels)
		{
			column.fill(chip8_color_unlit);
		}
	}

	uint8_t read_mem(uint16_t addr) {

		assert((addr >= 0x0000) and (addr < 0xFFFF) && "Address out of bounds");
		//assert((ram.at(addr) != ram_uninitialzed_value) && "Acessed memory from read operation is unitialized");

		return memory.at(addr);
	}

	void write_mem(uint16_t addr, uint8_t _data) {

		assert(addr >= program_starting_point && "writing to reserved area");
		assert((addr >= 0x0000) and (addr < 0xFFFF) && "Address out of bounds");
		//assert(rom.at(addr) == uninitialzed_value && "This adress is being overwritten");
		memory.at(addr) = _data;
	}
	

	//returns if load was successful
	bool load_bin_into_mem(const std::string& file_path) {
		
		std::ifstream file{ file_path, std::ios::binary };

		if (file.fail()) {
			return false;
		}

		reset();
		load_fonts_into_mem();
		uint8_t ch{};
		int i = program_starting_point;
		while (file >> std::noskipws >> ch) {
			memory[i] = ch;
			i++;
		}
		program_code_size = i;
		return true;
	}
};

