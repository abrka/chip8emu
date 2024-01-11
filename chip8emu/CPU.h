#pragma once

#include <bitset>
#include <cstdint>
#include <functional>
#include <map>
#include "Bus.h"
#include <optional>
#include <array>
#include "NumberSystemUtils.h"


constexpr uint16_t stack_high_addr{ 0x10 };
constexpr uint16_t program_starting_point{ 0x00};
constexpr uint8_t bytes_read_per_opcode{ 2 };
constexpr uint8_t opcode_hex_bit{ 1 };
constexpr uint8_t sprite_width{ 8 };

struct CPU {


	Bus* connected_bus;

	uint16_t pc{program_starting_point};
	uint16_t I{};
	//general purpose registers
	std::array<uint8_t, 16> V{};

	CPU(Bus* _connected_bus);

	void reset();
	void advance();

	std::stringstream dump_core() const;
	std::stringstream dump_registers() const;
	std::optional<std::stringstream> dump_current_instruction() const;
	std::stringstream dump_source(std::optional<uint16_t> size) const;

	void DISPLAY_SPRITE();
	void JUMP();
	void SET_REGISTER();
	void ADD_IMM();
};



