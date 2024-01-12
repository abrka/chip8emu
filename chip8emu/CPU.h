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

	void DISPLAY_SPRITE(uint8_t opcode_first_byte, uint8_t opcode_second_byte);
	void JUMP(uint8_t opcode_first_byte, uint8_t opcode_second_byte);
	void JUMP_V0(uint8_t opcode_first_byte, uint8_t opcode_second_byte);
	void SET_REGISTER(uint8_t opcode_first_byte, uint8_t opcode_second_byte);
	void ADD_IMM(uint8_t opcode_first_byte, uint8_t opcode_second_byte);
	void ADD_TWO_REG(uint8_t opcode_first_byte, uint8_t opcode_second_byte);
	void CLEAR_DISPLAY(uint8_t opcode_first_byte, uint8_t opcode_second_byte);
	void SET_I(uint8_t opcode_first_byte, uint8_t opcode_second_byte);
	void SKIP_NEXT(uint8_t opcode_first_byte, uint8_t opcode_second_byte);
	void SKIP_NEXT_IMM(uint8_t opcode_first_byte, uint8_t opcode_second_byte);
	void SKIP_NEXT_NOT_EQUAL_IMM(uint8_t opcode_first_byte, uint8_t opcode_second_byte);
	void SKIP_NEXT_KEY_NOT_PRESSED(uint8_t opcode_first_byte, uint8_t opcode_second_byte);
	void AND_REG(uint8_t opcode_first_byte, uint8_t opcode_second_byte);
	void WAIT_AND_LOAD_KEY(uint8_t opcode_first_byte, uint8_t opcode_second_byte);
	void RANDOM_BYTE_AND_KK(uint8_t opcode_first_byte, uint8_t opcode_second_byte);
	void STORE_BCD(uint8_t opcode_first_byte, uint8_t opcode_second_byte);
	void LOAD_REG_FROM_MEM(uint8_t opcode_first_byte, uint8_t opcode_second_byte);
};



