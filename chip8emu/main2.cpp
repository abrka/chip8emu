// Dear ImGui: standalone example application for SDL2 + SDL_Renderer
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

// Important to understand: SDL_Renderer is an _optional_ component of SDL2.
// For a multi-platform app consider using e.g. SDL+DirectX on Windows and SDL+OpenGL on Linux/OSX.

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "misc/cpp/imgui_stdlib.h"
#include <stdio.h>
#include <SDL.h>
#include <stdio.h>
#include <array>
#include <iostream>
#include "Bus.h"
#include "CPU.h"
#include <string>
#include "tinyfiledialogs/tinyfiledialogs.h"
#include <sstream>
#include <format>

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

enum class EmulatorState {
	Paused,
	Running
};
EmulatorState emulator_state = EmulatorState::Paused;

static uint32_t cpu_cycles_executed_per_frame = 2;

constexpr int window_starting_width = 1280;
constexpr int window_starting_height = 720;


static void draw_chip8_pixels(Bus* bus, SDL_Renderer* renderer);
void draw_memory_dump(CPU& cpu);
void step_cpu(CPU& cpu);
void pause_cpu();
void start_running_cpu();
void reset_cpu(Bus* bus, CPU& cpu, std::string& filepath);
void draw_registers(CPU& cpu);
void draw_source_code(Bus* bus, CPU& cpu);
static void Cleanup(SDL_Renderer* renderer, SDL_Window* window);

// Main code
int main(int, char**)
{
	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	// From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

	// Create window with SDL_Renderer graphics context
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+SDL_Renderer example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_starting_width, window_starting_height, window_flags);
	if (window == nullptr)
	{
		printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
		return -1;
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		SDL_Log("Error creating SDL_Renderer!");
		return 0;
	}
	//SDL_RendererInfo info;
	//SDL_GetRendererInfo(renderer, &info);
	//SDL_Log("Current SDL_Renderer: %s", info.name);
	SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SCALING, "1");
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != nullptr);

	// Our state
	bool show_demo_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	Bus* bus = new Bus{};
	CPU cpu{ bus };


	// Main loop
	bool done = false;
	while (!done)
	{
		switch (emulator_state)
		{
		case EmulatorState::Paused:
			break;
		case EmulatorState::Running:
			for (size_t cycle = 0; cycle < cpu_cycles_executed_per_frame; cycle++)
			{
				cpu.advance();
			}
		default:
			break;
		}



		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT) {
				done = true;
			}
			else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window)) {
				done = true;
			}
			else if (event.type == SDL_KEYDOWN) {
				bus->pressed_key = SDL_input_to_chip8_input(event.key.keysym.sym);
			}
			else if (event.type == SDL_KEYUP) {
				bus->pressed_key = {};
			}
		}

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);


		static std::string code_filepath{};

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window) {
			ImGui::ShowDemoWindow(&show_demo_window);
		}
		{
			ImGui::Begin("Debugger");

			static bool is_file_loaded{ false };
			
			static std::string error{};

			ImGui::Text(code_filepath.c_str());

			if (ImGui::Button("[ ] Load File")) {
				emulator_state = EmulatorState::Paused;
				bus->reset();
				cpu.reset();

				char const* selection = tinyfd_openFileDialog( // there is also a wchar_t version
					"Select file", // title
					NULL, // optional initial directory
					2, // number of filter patterns
					chip8_file_extensions, // char const * lFilterPatterns[2] = { "*.txt", "*.jpg" };
					NULL, // optional filter description
					0 // forbid multiple selections
				);
				if (selection != nullptr) {
					code_filepath = selection;
				}
				is_file_loaded = bus->load_bin_into_mem(code_filepath);

				if (not is_file_loaded) {
					error = "Couldnt load file";

				}
				else {
					error = "";

				}

			}
			ImGui::SameLine();
			if (ImGui::Button("|| Pause")) {
				pause_cpu();
			}

			ImGui::SameLine();
			if (ImGui::Button("|> Run")) {
				if (is_file_loaded) {
					start_running_cpu();
				}
				else {
					error = "file not loaded";
				}

			}

			ImGui::SameLine();
			if (ImGui::Button("O Restart")) {
				if (is_file_loaded) {
					reset_cpu(bus, cpu, code_filepath);
				}
				else {
					error += "\n No File is Loaded";
				}

			}

			ImGui::SameLine();
			if (ImGui::Button("-> Step")) {
				if (is_file_loaded) {
					step_cpu(cpu);
				}
				else {
					error = "file not loaded";
				}
			}


			ImGui::Text(error.c_str());
			ImGui::End();
		}
		{
			ImGui::Begin("Source Code");
			draw_source_code(bus, cpu);
			ImGui::End();
		}
		{
			ImGui::Begin("Memory");
			draw_memory_dump(cpu);
			ImGui::End();
		}

		ImGui::Begin("Registers, Stack and Keyboard");
		if (ImGui::BeginTabBar("Registers")) {

			if (ImGui::BeginTabItem("Registers")) {
				draw_registers(cpu);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Keyboard")) {
				if (bus->pressed_key.has_value()) {
					ImGui::Text("Pressed Key: %s", byte_to_hex_str(bus->pressed_key.value()).c_str());
				}
				else {
					ImGui::Text("Pressed Key: %s", "None");
				}
				ImGui::EndTabItem();
			}


			if (ImGui::BeginTabItem("Stack")) {
				ImGui::TextWrapped(cpu.dump_stack().str().c_str());
				ImGui::EndTabItem();
			}
		
			ImGui::EndTabBar();
		}
		ImGui::End();


		// Rendering
		SDL_RenderClear(renderer);

		//our code
	//	SDL_RenderSetLogicalSize(renderer, chip8_screen_width, chip8_screen_height);
		SDL_RenderSetScale(renderer, screen_scale , screen_scale);
		draw_chip8_pixels(bus, renderer);

		//imgui code
		ImGui::Render();
		int win_width{};
		int win_height{};
		SDL_GetWindowSize(window, &win_width, &win_height);
		SDL_RenderSetLogicalSize(renderer, win_width, win_height);
		SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		SDL_SetRenderDrawColor(renderer, neon.r, neon.g, neon.b, 0);
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(renderer);
	}

	// Cleanup
	Cleanup(renderer, window);

	return 0;
}

void draw_source_code(Bus* bus, CPU& cpu)
{
	std::string source_code{};
	for (int i = program_starting_point; i < program_starting_point + bus->program_code_size; i++) {
		uint8_t byte = bus->read_mem(i);
		if (i == cpu.pc) {
			source_code += std::format("0x{:02x}<- ", byte);
		}
		else {
			source_code += std::format("{} ", byte_to_hex_str(byte));
		}
	}
	ImGui::TextWrapped(source_code.c_str());
}

void draw_registers(CPU& cpu)
{
	ImGui::Text("PC: %s", word_to_hex_str(cpu.pc).c_str());
	ImGui::Text("I: %s", word_to_hex_str(cpu.I).c_str());
	ImGui::Text("Stack Ptr: %s", byte_to_hex_str(cpu.stack_ptr).c_str());
	ImGui::Text("Delay Timer: %s", byte_to_hex_str(cpu.delay_timer).c_str());
	ImGui::Text("Sound Timer: %s", byte_to_hex_str(cpu.sound_timer).c_str());
	for (size_t i = 0; i < cpu.V.size(); i++)
	{
		ImGui::Text("V[%s]: %s", byte_to_hex_str_ns(i).c_str(), byte_to_hex_str(cpu.V[i]).c_str());
	}
}

void reset_cpu(Bus* bus, CPU& cpu, std::string& filepath)
{
	bus->reset();
	cpu.reset();
	bus->load_bin_into_mem(filepath);
}

void start_running_cpu()
{
	emulator_state = EmulatorState::Running;
}

void pause_cpu()
{
	emulator_state = EmulatorState::Paused;
}

void step_cpu(CPU& cpu)
{
	cpu.advance();
}

void draw_memory_dump(CPU& cpu)
{


	for (size_t i = 0; i < size_of_mem - bytes_displayed_per_tab; i += bytes_displayed_per_tab)
	{
		if (ImGui::BeginTabBar("Mem tab")) {
			std::string tab_item_name = std::format("page {}", i / bytes_displayed_per_tab);
			if (ImGui::BeginTabItem(tab_item_name.c_str())) {
				ImGui::TextWrapped("%s", cpu.dump_core(i, i + bytes_displayed_per_tab).str().c_str());
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}



}



void draw_chip8_pixels(Bus* bus, SDL_Renderer* renderer)
{

	for (size_t y = 0; y < chip8_screen_height; y++)
	{
		for (size_t x = 0; x < chip8_screen_width; x++)
		{
			SDL_Rect rect{};
			rect.x = x + screen_offset_x;
			rect.y = y + screen_offset_y;
			rect.w = 1;
			rect.h = 1;
			chip8_color chip8_col = bus->pixels[y][x];
			rgba_color rect_col = chip8_color_to_rgba(chip8_col);


			SDL_SetRenderDrawColor(renderer, rect_col.r, rect_col.g, rect_col.b, rect_col.a);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

void Cleanup(SDL_Renderer* renderer, SDL_Window* window)
{
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
