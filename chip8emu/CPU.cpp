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
#include <cstdlib>
#include <ctime>

constexpr int infinity = std::numeric_limits<int>::max();


CPU::CPU(Bus* _connected_bus) : connected_bus{ _connected_bus }
{
	std::srand(std::time(nullptr));
}

void CPU::reset()
{
	V.fill(0x0);
	pc = program_starting_point;
	stack_ptr = 0;
	I = 0;
	delay_timer = 0;
	sound_timer = 0;

}


void CPU::advance()
{

	if (delay_timer != 0) {
		delay_timer--;
	}
	if (sound_timer != 0) {
		sound_timer--;
	}

	uint8_t opcode_first_byte = connected_bus->memory.at(pc);
	uint8_t opcode_second_byte = connected_bus->memory.at(pc + 1);

	if (higher_nibble(opcode_first_byte) == 0xD) {
		DISPLAY_SPRITE(opcode_first_byte, opcode_second_byte);
	}
	else if (higher_nibble(opcode_first_byte) == 0x1) {
		JUMP(opcode_first_byte, opcode_second_byte);
	}
	else if (higher_nibble(opcode_first_byte) == 0x2) {
		CALL_SUBROUTINE(opcode_first_byte, opcode_second_byte);
	}
	else if ((opcode_first_byte == 0x00) and (opcode_second_byte == 0xEE)) {
		RET_FROM_SR(opcode_first_byte, opcode_second_byte);
	}
	else if (higher_nibble(opcode_first_byte) == 0x6) {
		SET_REGISTER(opcode_first_byte, opcode_second_byte);
	}
	else if (higher_nibble(opcode_first_byte) == 0xB) {
		JUMP_V0(opcode_first_byte, opcode_second_byte);
	}
	else if (higher_nibble(opcode_first_byte) == 0x7) {
		ADD_IMM(opcode_first_byte, opcode_second_byte);
	}
	else if ((higher_nibble(opcode_first_byte) == 0x8) and (lower_nibble(opcode_second_byte) == 0x4)) {
		ADD_TWO_REG(opcode_first_byte, opcode_second_byte);
	}
	else if ((higher_nibble(opcode_first_byte) == 0x8) and (lower_nibble(opcode_second_byte) == 0x5)) {
		SUB_TWO_REG(opcode_first_byte, opcode_second_byte);
	}
	else if ((higher_nibble(opcode_first_byte) == 0x8) and (lower_nibble(opcode_second_byte) == 0x7)) {
		SUB_TWO_REG_FLIPPED(opcode_first_byte, opcode_second_byte);
	}
	else if ((higher_nibble(opcode_first_byte) == 0x8) and (lower_nibble(opcode_second_byte) == 0x6)) {
		SHR_REG(opcode_first_byte, opcode_second_byte);
	}
	else if ((higher_nibble(opcode_first_byte) == 0x8) and (lower_nibble(opcode_second_byte) == 0xE)) {
		SHL_REG(opcode_first_byte, opcode_second_byte);
	}
	else if ((higher_nibble(opcode_first_byte) == 0x8) and (lower_nibble(opcode_second_byte) == 0x0)) {
		STORE_REG_IN_REG(opcode_first_byte, opcode_second_byte);
	}
	else if ((higher_nibble(opcode_first_byte) == 0x8) and (lower_nibble(opcode_second_byte) == 0x1)) {
		OR_REG(opcode_first_byte, opcode_second_byte);
	}
	else if ((higher_nibble(opcode_first_byte) == 0x8) and (lower_nibble(opcode_second_byte) == 0x3)) {
		XOR_REG(opcode_first_byte, opcode_second_byte);
	}
	else if (higher_nibble(opcode_first_byte) == 0xA) {
		SET_I(opcode_first_byte, opcode_second_byte);
	}
	else if ((higher_nibble(opcode_first_byte) == 0xF) and (opcode_second_byte == 0x1E)) {
		ADD_I(opcode_first_byte, opcode_second_byte);
	}
	else if (higher_nibble(opcode_first_byte) == 0x5) {
		SKIP_NEXT(opcode_first_byte, opcode_second_byte);
	}
	else if (higher_nibble(opcode_first_byte) == 0x9) {
		SKIP_NEXT_NOT_EQUAL(opcode_first_byte, opcode_second_byte);
	}
	else if (higher_nibble(opcode_first_byte) == 0x3) {
		SKIP_NEXT_IMM(opcode_first_byte, opcode_second_byte);
	}
	else if (higher_nibble(opcode_first_byte) == 0x4) {
		SKIP_NEXT_NOT_EQUAL_IMM(opcode_first_byte, opcode_second_byte);
	}
	else if ((higher_nibble(opcode_first_byte) == 0xE) and (opcode_second_byte == 0x9E)) {
		SKIP_NEXT_KEY_PRESSED(opcode_first_byte, opcode_second_byte);
	}
	else if ((higher_nibble(opcode_first_byte) == 0xE) and (opcode_second_byte == 0xA1)) {
		SKIP_NEXT_KEY_NOT_PRESSED(opcode_first_byte, opcode_second_byte);
	}
	else if ((higher_nibble(opcode_first_byte) == 0xF) and (opcode_second_byte == 0x33)) {
		STORE_BCD(opcode_first_byte, opcode_second_byte);
	}
	else if ((higher_nibble(opcode_first_byte) == 0xF) and (opcode_second_byte == 0x55)) {
		SAVE_REG_TO_MEM(opcode_first_byte, opcode_second_byte);
	}
	else if ((higher_nibble(opcode_first_byte) == 0xF) and (opcode_second_byte == 0x65)) {
		LOAD_REG_FROM_MEM(opcode_first_byte, opcode_second_byte);
	}
	else if (higher_nibble(opcode_first_byte) == 0xC) {
		RANDOM_BYTE_AND_KK(opcode_first_byte, opcode_second_byte);
	}
	else if ((higher_nibble(opcode_first_byte) == 0x8) and (lower_nibble(opcode_second_byte) == 0x2)) {
		AND_REG(opcode_first_byte, opcode_second_byte);
	}
	else if (opcode_first_byte == 0x00 and opcode_second_byte == 0xE0) {
		CLEAR_DISPLAY(opcode_first_byte, opcode_second_byte);
	}
	else if (higher_nibble(opcode_first_byte) == 0xF and opcode_second_byte == 0x0A) {
		WAIT_AND_LOAD_KEY(opcode_first_byte, opcode_second_byte);
	}
	else if (higher_nibble(opcode_first_byte) == 0xF and opcode_second_byte == 0x29) {
		SET_I_TO_LOC_OF_FONT(opcode_first_byte, opcode_second_byte);
	}
	else if (higher_nibble(opcode_first_byte) == 0xF and opcode_second_byte == 0x18) {
		SET_SOUND_TIMER(opcode_first_byte, opcode_second_byte);
	}
	else if (higher_nibble(opcode_first_byte) == 0xF and opcode_second_byte == 0x15) {
		SET_DELAY_TIMER(opcode_first_byte, opcode_second_byte);
	}
	else if (higher_nibble(opcode_first_byte) == 0xF and opcode_second_byte == 0x07) {
		SET_VX_TO_DELAY_TIMER(opcode_first_byte, opcode_second_byte);
	}
	else {
		uint16_t full_opcode = TwoByteToOneWord(opcode_second_byte, opcode_first_byte);
		std::cout << "opcode: " << std::hex << full_opcode << std::endl;
		assert(false && "opcode not supported");
	}
	pc += bytes_read_per_opcode;

}

std::stringstream CPU::dump_core() const
{
	std::stringstream output{};
	for (auto& byte : connected_bus->memory)
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
		assert(size <= connected_bus->memory.size() && "size of data provided in dump source is bigger than the size of rom");
	}

	std::stringstream output{};
	for (int i = 0; i < size.value_or(connected_bus->memory.size()); i++)
	{
		uint8_t byte = connected_bus->read_mem(i);
		output << std::setw(4) << "0x" << std::hex << (int)byte;
	}
	return output;
}

void CPU::DISPLAY_SPRITE(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t instruction_byte_start = opcode_first_byte;
	uint8_t instruction_byte_end = opcode_second_byte;

	uint8_t sprite_height = get_hex_nth_digit(instruction_byte_end, 0);

	uint8_t x_offset_reg = get_hex_nth_digit(instruction_byte_start, 0);
	uint8_t y_offset_reg = get_hex_nth_digit(instruction_byte_end, 1);

	uint8_t x_offset = V[x_offset_reg];
	uint8_t y_offset = V[y_offset_reg];

	for (size_t y = 0; y < sprite_height; y++)
	{
		uint8_t pixel_y_coord = (y + y_offset) % chip8_screen_height;
		std::bitset<8> row_pixels = reverse_bitset(std::bitset<8>{ connected_bus->read_mem(y + I) });


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

void CPU::JUMP(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{

	uint8_t jump_adress_first_hex = lower_nibble(opcode_first_byte);

	uint16_t jump_adress = jump_adress_first_hex * pow(16, 2) + opcode_second_byte;

	pc = jump_adress - bytes_read_per_opcode; //bytes per opcode needs to subtracted since it will be added in advance funciton

	//std::cout << "jump 0x" << std::hex << (int)jump_adress << std::endl;
}

void CPU::JUMP_V0(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t jump_adress_first_hex = lower_nibble(opcode_first_byte);

	uint16_t jump_adress = (jump_adress_first_hex * pow(16, 2) + opcode_second_byte) + V[0];

	pc = jump_adress - bytes_read_per_opcode; //bytes per opcode needs to subtracted since it will be added in advance funciton

}

void CPU::SET_REGISTER(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{

	uint8_t which_reg_to_set = get_hex_nth_digit(opcode_first_byte, 0);
	uint8_t value = opcode_second_byte;

	V[which_reg_to_set] = value;
}

void CPU::ADD_IMM(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{

	uint8_t reg_to_add_to = get_hex_nth_digit(opcode_first_byte, 0);
	uint8_t value = opcode_second_byte;

	V[reg_to_add_to] += value;
}

void CPU::ADD_TWO_REG(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t& VX = V[lower_nibble(opcode_first_byte)];
	uint8_t VY = V[higher_nibble(opcode_second_byte)];
	VX = VX + VY;
	if (VX < VY) {
		V[0xF] = 1;
	}
	else {
		V[0xF] = 0;
	}
}

void CPU::SUB_TWO_REG(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t& VX = V[lower_nibble(opcode_first_byte)];
	uint8_t prev_VX = VX;
	uint8_t VY = V[higher_nibble(opcode_second_byte)];

	VX -= VY;

	if (VX > prev_VX) {
		V[0xF] = 0;
	}
	else {
		V[0xF] = 1;
	}


}

void CPU::SUB_TWO_REG_FLIPPED(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t& VX = V[lower_nibble(opcode_first_byte)];
	uint8_t VY = V[higher_nibble(opcode_second_byte)];


	VX = VY - VX;

	if (VY > VX) {
		V[0xF] = 1;
	}
	else {
		V[0xF] = 0;
	}


}

void CPU::SHR_REG(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t& VX = V[lower_nibble(opcode_first_byte)];
	uint8_t lsb_of_VX = VX & 1;
	uint8_t& VY = V[higher_nibble(opcode_second_byte)];

	VX >>= 1;

	V[0xF] = lsb_of_VX;


}

void CPU::SHL_REG(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t& VX = V[lower_nibble(opcode_first_byte)];
	uint8_t msb_of_VX = (VX >> 7) & 1;
	uint8_t& VY = V[higher_nibble(opcode_second_byte)];

	VX <<= 1;

	V[0xF] = msb_of_VX;
}

void CPU::CLEAR_DISPLAY(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	for (size_t y = 0; y < chip8_screen_height; y++)
	{
		for (size_t x = 0; x < chip8_screen_width; x++)
		{
			connected_bus->pixels[y][x] = chip8_color_unlit;
		}
	}
}

void CPU::SET_I(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{

	uint8_t val_last_hex = get_hex_nth_digit(opcode_first_byte, 0);

	uint16_t value = val_last_hex * pow(16, 2) + opcode_second_byte;

	I = value;
}

void CPU::ADD_I(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t VX = V[lower_nibble(opcode_first_byte)];
	I += VX;
}

void CPU::SKIP_NEXT(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{

	uint8_t VX = V[get_hex_nth_digit(opcode_first_byte, 0)];
	uint8_t VY = V[get_hex_nth_digit(opcode_second_byte, 1)];

	if (VX == VY) {
		pc += bytes_read_per_opcode;
	}
}

void CPU::SKIP_NEXT_NOT_EQUAL(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t VX = V[get_hex_nth_digit(opcode_first_byte, 0)];
	uint8_t VY = V[get_hex_nth_digit(opcode_second_byte, 1)];

	if (VX != VY) {
		pc += bytes_read_per_opcode;
	}
}

void CPU::SKIP_NEXT_IMM(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{

	uint8_t VX = V[get_hex_nth_digit(opcode_first_byte, 0)];

	if (VX == opcode_second_byte) {
		pc += bytes_read_per_opcode;
	}
}

void CPU::SKIP_NEXT_NOT_EQUAL_IMM(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t VX = V[lower_nibble(opcode_first_byte)];

	if (VX != opcode_second_byte) {
		pc += bytes_read_per_opcode;
	}
}

void CPU::SKIP_NEXT_KEY_PRESSED(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t VX = V[lower_nibble(opcode_first_byte)];

	if (not connected_bus->pressed_key.has_value()) {
		pc += bytes_read_per_opcode;
		return;
	}
	if (VX == connected_bus->pressed_key.value()) {
		pc += bytes_read_per_opcode;
	}
}

void CPU::SKIP_NEXT_KEY_NOT_PRESSED(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t VX = V[lower_nibble(opcode_first_byte)];

	if (not connected_bus->pressed_key.has_value()) {
		pc += bytes_read_per_opcode;
		return;
	}
	if (VX != connected_bus->pressed_key.value()) {
		pc += bytes_read_per_opcode;
	}
}

void CPU::AND_REG(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t& VX = V[lower_nibble(opcode_first_byte)];
	uint8_t VY = V[higher_nibble(opcode_second_byte)];

	VX = VX & VY;
}

void CPU::OR_REG(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t& VX = V[lower_nibble(opcode_first_byte)];
	uint8_t VY = V[higher_nibble(opcode_second_byte)];

	VX = VX | VY;
}

void CPU::XOR_REG(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t& VX = V[lower_nibble(opcode_first_byte)];
	uint8_t VY = V[higher_nibble(opcode_second_byte)];

	VX = VX ^ VY;
}

void CPU::WAIT_AND_LOAD_KEY(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	if (not connected_bus->pressed_key.has_value()) {
		pc -= bytes_read_per_opcode; //since bytes read will be added later we subtract it here. this means the pc will stay at the same value until something is pressed
		return;
	}
	uint8_t& VX = V[lower_nibble(opcode_first_byte)];
	VX = connected_bus->pressed_key.value();
}

void CPU::RANDOM_BYTE_AND_KK(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t random_value = std::rand() % (255 + 1);
	uint8_t& VX = V[lower_nibble(opcode_first_byte)];
	uint8_t KK = opcode_second_byte;

	VX = random_value & KK;
}

void CPU::STORE_BCD(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	
	uint8_t X = lower_nibble(opcode_first_byte);
	uint8_t vX;
	vX = V[X];
	connected_bus->memory[I] = (vX - (vX % 100)) / 100;
	vX -= connected_bus->memory[I] * 100;
	connected_bus->memory[I + 1] = (vX - (vX % 10)) / 10;
	vX -= connected_bus->memory[I + 1] * 10;
	connected_bus->memory[I + 2] = vX;

	/*uint8_t VX = V[lower_nibble(opcode_first_byte)];
	
	uint8_t hundreds_digit = get_digit(VX, 2);
	uint8_t tens_digit = get_digit(VX, 1);
	uint8_t ones_digit = get_digit(VX, 0);

	connected_bus->write_mem(I + 0, hundreds_digit);
	connected_bus->write_mem(I + 1, tens_digit);
	connected_bus->write_mem(I + 2, ones_digit);
	*/

	/*uint8_t VX = V[lower_nibble(opcode_first_byte)];
	uint8_t hundreds_digit = VX / 100;
	uint8_t tens_digit = (VX / 10) % 10;
	uint8_t ones_digit = (VX % 100) % 10;

	connected_bus->write_mem(I, hundreds_digit);
	connected_bus->write_mem(I + 1, tens_digit);
	connected_bus->write_mem(I + 2, ones_digit);*/

	//std::cout << std::bitset<8>{hundreds_digit} << " "<<  std::bitset<8>{tens_digit} << " "<<  std::bitset<8>{ones_digit} << std::endl;
	std::cout << (int)V[X] << std::endl;
	std::cout << std::hex << (int)X << std::endl;
	std::cout << std::hex << TwoByteToOneWord(opcode_second_byte, opcode_first_byte) << std::endl;
	std::cout << std::bitset<8>{connected_bus->memory[I]} << " " << std::bitset<8>{connected_bus->memory[I + 1]} << " " << std::bitset<8>{connected_bus->memory[I + 2]} << std::endl;

	//connected_bus->memory[(I + 0) & 0xfff] = (VX % 1000) / 100;
	//connected_bus->memory[(I + 1) & 0xfff] = (VX % 100) / 10;
	//connected_bus->memory[(I + 2) & 0xfff] = VX % 10;
}

void CPU::SAVE_REG_TO_MEM(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t X = lower_nibble(opcode_first_byte);
	for (uint8_t i = 0; i <= X; i++)
	{
		connected_bus->write_mem(I + i, V[i]);
	}
}

void CPU::LOAD_REG_FROM_MEM(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	const uint8_t X = lower_nibble(opcode_first_byte);
	for (uint8_t i = 0; i <= X; i++)
	{
		V[i] = connected_bus->read_mem(I + i);
	}
	
	std::cout << "x is" << (int)X << std::endl;
	std::cout << (int)V[0x2] << std::endl;
	std::cout << (int)connected_bus->read_mem(I + 0) << std::endl;
	std::cout << (int)connected_bus->read_mem(I + 1) << std::endl;
	std::cout << (int)connected_bus->read_mem(I + 2)<< std::endl;
	std::cout << "load from mem \n";
	
	
}

void CPU::SET_I_TO_LOC_OF_FONT(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t which_font = lower_nibble(opcode_first_byte);
	uint8_t adress_of_font = connected_bus->get_adress_of_font(which_font);
	I = adress_of_font;

	std::cout << "whcih font: " << (int)which_font << "adr " << (int)adress_of_font << std::endl;
}

void CPU::SET_SOUND_TIMER(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t VX = V[lower_nibble(opcode_first_byte)];
	sound_timer = VX;
}

void CPU::SET_DELAY_TIMER(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t VX = V[lower_nibble(opcode_first_byte)];
	delay_timer = VX;
}

void CPU::SET_VX_TO_DELAY_TIMER(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t& VX = V[lower_nibble(opcode_first_byte)];
	VX = delay_timer;

}

void CPU::STORE_REG_IN_REG(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint8_t& VX = V[get_hex_nth_digit(opcode_first_byte, 0)];
	uint8_t VY = V[get_hex_nth_digit(opcode_second_byte, 1)];
	VX = VY;
}

void CPU::CALL_SUBROUTINE(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	stack_ptr++;
	connected_bus->stack[stack_ptr] = pc;

	uint8_t n = lower_nibble(opcode_first_byte);
	uint8_t nn = opcode_second_byte;
	uint16_t nnn = TwoByteToOneWord(nn, n);
	pc = nnn - bytes_read_per_opcode; // bytes per opcode needs to be subtracted since it will be added in the execute function

}

void CPU::RET_FROM_SR(uint8_t opcode_first_byte, uint8_t opcode_second_byte)
{
	uint16_t top_of_stack = connected_bus->stack[stack_ptr];
	pc = top_of_stack;

	stack_ptr--;
}

