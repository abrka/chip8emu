#pragma once

#include <bitset>
#include <cstdint>
#include <functional>
#include <map>
#include "Bus.h"
#include <optional>
#include <array>


static uint16_t TwoByteToOneWord(uint8_t lsb, uint8_t msb) {
	return (msb << 8) + lsb;
}

static int8_t uint8_to_int8(uint8_t x)
{
	return ((x >= (1 << 7))
		? -(UINT8_MAX - x + 1)
		: x);
}
static bool bit_test(uint8_t flags, uint8_t mask) {
	return flags & mask;
}

static uint8_t get_hex_nth_digit(uint8_t hex, uint8_t nth_digit) {
	return (hex >> (nth_digit << 2)) & 0xf;
}
template<std::size_t N>
static std::bitset<N> reverse_bitset(std::bitset<N> b) {
	for (std::size_t i = 0; i < N / 2; ++i) {
		bool t = b[i];
		b[i] = b[N - i - 1];
		b[N - i - 1] = t;
	}
	return b;
}

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
};



