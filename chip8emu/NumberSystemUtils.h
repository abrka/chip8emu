#pragma once
#include <bitset>
#include <cstdint>
#include <math.h>

static int get_digit(int number, int digit)
{
	return (number / ((int)pow(10, digit)) % 10);
}


static uint8_t bcd_to_dec(uint8_t val)
{
	return ((val / 16 * 10) + (val % 16));
}

static uint8_t dec_to_bcd(uint8_t val)
{
	return ((val / 10 * 16) + (val % 10));
}

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
static uint8_t lower_nibble(uint8_t data) {
	return get_hex_nth_digit(data, 0);
}
static uint8_t higher_nibble(uint8_t data) {
	return get_hex_nth_digit(data, 1);
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
