#include "CPU.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <array>
#include <vector>
#include <cstddef>
#include <string>
#include <sstream>
#include <cassert>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <map>
#include "Bus.h"
#include <limits>

constexpr int infinity = std::numeric_limits<int>::max();


CPU::CPU(Bus* _connected_bus) : connected_bus{ _connected_bus }
{

}

void CPU::reset()
{
	for (auto& reg : V) {
		reg = 0x00;
	}
	pc = program_starting_point;

}


void CPU::advance()
{

	uint8_t opcode = connected_bus->rom.at(pc);

	if (get_hex_nth_digit(opcode, opcode_hex_bit) == 0xD) {
		DISPLAY_SPRITE();
	}
	else if (get_hex_nth_digit(opcode, opcode_hex_bit) == 0x1) {
		JUMP();
	}
	else if (get_hex_nth_digit(opcode, opcode_hex_bit) == 0x6) {
		SET_REGISTER();
	}
	else if (get_hex_nth_digit(opcode, opcode_hex_bit) == 0x7) {
		ADD_IMM();
	}
	else {
		std::cout << "opcode: " << (int)opcode << std::endl;
		assert(false && "opcode not supported");
	}
	pc += bytes_read_per_opcode;

}

std::stringstream CPU::dump_core() const
{
	std::stringstream output{};
	for (auto& byte : connected_bus->ram)
	{
		output << std::setw(4) << "0x" << std::hex << (int)byte;
	}
	return output;
}

std::stringstream CPU::dump_registers() const
{
	std::stringstream output{};
	for (size_t i = 0; i < V.size(); i++)
	{
		output << "V[" << std::hex << i << "]" << " value:" << V[i] << "\n";
	}
	
	output << "program counter " << "0x" << std::hex << (int)pc << "\n";
	return output;
}

std::optional<std::stringstream> CPU::dump_current_instruction() const
{

	/*uint8_t instruction = connected_bus->rom.at(pc);

	if (not operation_lookup_table.contains(instruction)) {
		return {};
	}

	std::string insturction_name = operation_lookup_table.at(instruction).operation_name;*/

	std::stringstream output{};
	//output << "program counter: " << "0x" << std::hex << (int)pc << "\n";
	//output << "instruction: " << "0x" << std::hex << (int)instruction << " (" << insturction_name << ")" << "\n";

	return output;
}

std::stringstream CPU::dump_source(std::optional<uint16_t> size = {}) const
{
	/*std::stringstream output{};
	for (int i =0; auto& byte : connected_bus->rom)
	{
		if (not( i < size.value_or(infinity))) {
			return output;
		}
		output << std::setw(4) << "0x" << std::hex << (int)byte;
		i++;
	}
	return output;*/

	if (size.has_value()) {
		assert(size <= connected_bus->rom.size() && "size of data provided in dump source is bigger than the size of rom");
	}

	std::stringstream output{};
	for (int i = 0; i < size.value_or(connected_bus->rom.size()); i++)
	{
		uint8_t byte = connected_bus->read_rom(i);
		output << std::setw(4) << "0x" << std::hex << (int)byte;
	}
	return output;
}

void CPU::DISPLAY_SPRITE()
{
	uint8_t instruction_byte_start = connected_bus->read_rom(pc);
	uint8_t instruction_byte_end = connected_bus->read_rom(pc + 1);

	uint8_t sprite_height = get_hex_nth_digit(instruction_byte_end, 0);

	uint8_t x_offset_reg = get_hex_nth_digit(instruction_byte_start, 0);
	uint8_t y_offset_reg = get_hex_nth_digit(instruction_byte_end, 1);

	uint8_t x_offset = V[x_offset_reg];
	uint8_t y_offset = V[y_offset_reg];

	for (size_t column = I; column < sprite_height; column++)
	{
		uint8_t pixel_y_coord = (column - I + y_offset) % chip8_screen_height;
		std::bitset<8> row_pixels = reverse_bitset(std::bitset<8>{ connected_bus->read_ram(column) });


		for (size_t x = 0; x < row_pixels.size(); x++)
		{
			uint8_t pixel_x_coord = (x + x_offset) % chip8_screen_width;
			chip8_color prev_color = connected_bus->pixels[pixel_y_coord][pixel_x_coord];
			chip8_color result_color = prev_color ^ row_pixels[x];
			connected_bus->pixels[pixel_y_coord][pixel_x_coord] = result_color;

			if (prev_color == chip8_color_lit and result_color == chip8_color_unlit) {
				V[0xF] = 1;
			}
			else {
				V[0xF] = 0;
			}
			
		}
		
	}
	//std::cout << std::dec << "draw sprite at x:" << (unsigned int)x_offset << " y:" << (unsigned int)y_offset << " sprite height "<< (unsigned int)sprite_height << std::endl;
}

void CPU::JUMP()
{

	uint8_t jump_address_first_two_hex = connected_bus->read_rom(pc + 1);
	uint8_t jump_adress_last_hex = get_hex_nth_digit(connected_bus->read_rom(pc),0);

	uint16_t jump_adress = jump_adress_last_hex * pow(16, 2) + jump_address_first_two_hex;

	pc = jump_adress - bytes_read_per_opcode; //bytes per opcode needs to subtracted since it will be added in advance funciton

	//std::cout << "jump 0x" << std::hex << (int)jump_adress << std::endl;
}

void CPU::SET_REGISTER()
{
	uint8_t current_instruction_first_byte = connected_bus->read_rom(pc);
	uint8_t current_instruction_second_byte = connected_bus->read_rom(pc + 1);

	uint8_t which_reg_to_set = get_hex_nth_digit(current_instruction_first_byte, 0);
	uint8_t value = current_instruction_second_byte;

	V[which_reg_to_set] = value;
}

void CPU::ADD_IMM()
{
	uint8_t current_instruction_first_byte = connected_bus->read_rom(pc);
	uint8_t current_instruction_second_byte = connected_bus->read_rom(pc + 1);

	uint8_t reg_to_add_to = get_hex_nth_digit(current_instruction_first_byte, 0);
	uint8_t value = current_instruction_second_byte;

	V[reg_to_add_to] += value;
}

