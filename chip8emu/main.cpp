#include <stdio.h>
#include <SDL.h>
#include <array>
#include <iostream>
#include "Bus.h"
#include "CPU.h"
#include "main.h"

// Define MAX and MIN macros
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// Define screen dimensions
static int SCREEN_WIDTH = 800;
static int SCREEN_HEIGHT = 600;


struct rgba_color {
	uint8_t r, g, b, a;
};
constexpr rgba_color neon = { 57, 255, 20, 255 };
constexpr rgba_color black = { 0,0,0,255 };
constexpr rgba_color white = { 255,255,255,255 };

static rgba_color chip8_color_to_rgba(chip8_color col) {
	if (col == true) {
		return neon;
	}
	else {
		return black;
	}
}
void DrawChip8Pixels(Bus* bus, SDL_Renderer* renderer);

template<typename T>
static T map_range(T OldValue, T OldMin, T OldMax, T NewMax, T NewMin) {
	return (((OldValue - OldMin) * (NewMax - NewMin)) / (OldMax - OldMin)) + NewMin;
}

static float map_chip8_width_to_window_width(float chip8_coord) {
	return map_range(chip8_coord, 0.0f, (float)chip8_screen_width, (float)SCREEN_WIDTH, 0.0f);
}

static float map_chip8_height_to_window_height(float chip8_coord) {
	return map_range(chip8_coord, 0.0f, (float)chip8_screen_height, (float)SCREEN_HEIGHT, 0.0f);
}

int main(int argc, char* argv[])
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not be initialized!\n"
			"SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

#if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
	// Disable compositor bypass
	if (!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0"))
	{
		printf("SDL can not disable compositor bypass!\n");
		return 0;
	}
#endif

	// Create window
	SDL_Window* window = SDL_CreateWindow("Basic C SDL project",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!window)
	{
		printf("Window could not be created!\n"
			"SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	// Create renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		printf("Renderer could not be created!\n"
			"SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	SDL_RenderSetLogicalSize(renderer, chip8_screen_width, chip8_screen_height);

	Bus* bus = new Bus{};
	CPU cpu{ bus };

	//cpu.V[0x4] = 32;
	bus->write_rom(0, 0x74);
	bus->write_rom(1, 2);

	//cpu.V[0x3] = 15;
	bus->write_rom(2, 0x63);
	bus->write_rom(3, 15);


	bus->write_rom(4, 0xD4);
	bus->write_rom(5, 0x35);


	bus->write_rom(6, 0x10);
	bus->write_rom(7, 0x00);

	cpu.I = 0x0;
	bus->write_ram(0x0, 0xF0);
	bus->write_ram(0x1, 0x10);
	bus->write_ram(0x2, 0x20);
	bus->write_ram(0x3, 0x40);
	bus->write_ram(0x4, 0x40);


	// Event loop exit flag
	bool quit = false;
	// Event loop
	while (!quit)
	{
		SDL_Event e;

		// Wait indefinitely for the next available event
		SDL_WaitEvent(&e);

		// User requests quit
		if (e.type == SDL_QUIT)
		{
			quit = true;
		}

		cpu.advance();

		// Initialize renderer color white for the background
		SDL_SetRenderDrawColor(renderer, neon.r, neon.g, neon.b, neon.a);

		// Clear screen
		SDL_RenderClear(renderer);

		DrawChip8Pixels(bus, renderer);

		// Update screen
		SDL_RenderPresent(renderer);
	}

	// Destroy renderer
	SDL_DestroyRenderer(renderer);

	// Destroy window
	SDL_DestroyWindow(window);


	// Quit SDL
	SDL_Quit();

	return 0;
}

void DrawChip8Pixels(Bus* bus, SDL_Renderer* renderer)
{
	for (size_t y = 0; y < chip8_screen_height; y++)
	{
		for (size_t x = 0; x < chip8_screen_width; x++)
		{
			SDL_Rect rect{};
			rect.x = x;
			rect.y = y;
			rect.w = 10;
			rect.h = 10;
			chip8_color chip8_col = bus->pixels[y][x];
			rgba_color rect_col = chip8_color_to_rgba(chip8_col);


			SDL_SetRenderDrawColor(renderer, rect_col.r, rect_col.g, rect_col.b, rect_col.a);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}
