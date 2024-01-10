#pragma once
#include <cstdint>
#include <array>
#include <assert.h>
#include <functional>
#include <iostream>
#include <fstream>
#include <optional>


constexpr uint8_t uninitialzed_value = 0xFF;
constexpr int chip8_screen_width = 64;
constexpr int chip8_screen_height = 32;
using chip8_color = bool;
constexpr bool chip8_color_lit = true;
constexpr bool chip8_color_unlit = false;

struct Bus {

	std::array<uint8_t, 0xFFFF> rom{};
	std::array<uint8_t, 0xFFFF> ram{};

	std::array<std::array<chip8_color, chip8_screen_width>, chip8_screen_height> pixels{};

	std::optional<uint16_t> program_code_size = {};

	Bus() {
		ram.fill(uninitialzed_value);
		rom.fill(uninitialzed_value);
	}

	uint8_t read_ram(uint16_t addr) {

		assert((addr >= 0x0000) and (addr < 0xFFFF) && "Address out of bounds");
		//assert((ram.at(addr) != ram_uninitialzed_value) && "Acessed memory from read operation is unitialized");

		return ram.at(addr);
	}

	void write_ram(uint16_t addr, uint8_t _data) {

		assert((addr >= 0x0000) and (addr < 0xFFFF) && "Address out of bounds");
		ram.at(addr) = _data;
	}

	void clear_rom() {
		rom.fill(uninitialzed_value);
	}

	uint8_t read_rom(uint16_t addr) {

		assert((addr >= 0x0000) and (addr < 0xFFFF) && "Address out of bounds");
		//assert((ram.at(addr) != ram_uninitialzed_value) && "Acessed memory from read operation is unitialized");

		return rom.at(addr);
	}

	void write_rom(uint16_t addr, uint8_t _data) {

		assert((addr >= 0x0000) and (addr < 0xFFFF) && "Address out of bounds");
		assert(rom.at(addr) == uninitialzed_value && "This adress is being overwritten");
		rom.at(addr) = _data;
	}
	

	//returns if load was successful
	bool load_binary_file_into_rom(const std::string& file_path) {
		
		std::ifstream file{ file_path, std::ios::binary };

		if (file.fail()) {
			return false;
		}

		clear_rom();
		uint8_t ch{};
		int i = 0;
		while (file >> std::noskipws >> ch) {
			write_rom(i, ch);
			i++;
		}
		program_code_size = i;
		return true;
	}
};

