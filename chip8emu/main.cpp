/*
#include <stdio.h>
#include <SDL.h>
#include <array>
#include <iostream>
#include "Bus.h"
#include "CPU.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

// Define screen dimensions
static int SCREEN_WIDTH = 800;
static int SCREEN_HEIGHT = 600;


void DrawChip8Pixels(Bus* bus, SDL_Renderer* renderer);



int main(int argc, char* argv[])
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) < 0)
	{
		printf("SDL could not be initialized!\n"
			"SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

	// From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

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

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);


	SDL_RenderSetLogicalSize(renderer, chip8_screen_width, chip8_screen_height);

	Bus* bus = new Bus{};
	CPU cpu{ bus };
	bool success = bus->load_bin_into_mem("data/Astro Dodge [Revival Studios, 2008].ch8");
	if (not success) {
		std::cout << "couldnt load file";
		return -1;
	}

	bool show_demo_window = true;

	// Event loop exit flag
	bool quit = false;
	// Event loop
	while (!quit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			ImGui_ImplSDL2_ProcessEvent(&e);
			//User requests quitf
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN) {
				
				bus->pressed_key = SDL_input_to_chip8_input(e.key.keysym.sym);
			}
			else if (e.type == SDL_KEYUP) {
				bus->pressed_key = {};
			}
		}


		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		cpu.advance();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (true)
			ImGui::ShowDemoWindow(&show_demo_window);


		

		

		DrawChip8Pixels(bus, renderer);

		SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);

		ImGui::Render();

		// Initialize renderer color white for the background
		//SDL_SetRenderDrawColor(renderer, neon.r, neon.g, neon.b, neon.a);

		// Clear screen
		SDL_RenderClear(renderer);



		// Update screen
		SDL_RenderPresent(renderer);
	}

	// Cleanup
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

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
*/