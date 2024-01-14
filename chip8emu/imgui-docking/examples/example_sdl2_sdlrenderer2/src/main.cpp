// Dear ImGui: standalone example application for SDL2 + SDL_Renderer
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

// Important to understand: SDL_Renderer is an _optional_ component of SDL2.
// For a multi-platform app consider using e.g. SDL+DirectX on Windows and SDL+OpenGL on Linux/OSX.

/*#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <stdio.h>
#include <SDL.h>
#include "Property.h"
#include "PropertyContainer.h"
#include "TreeNode.h"
#include "AssetBrowser.h"
#include "TreeNodeRenderer.h"
#include "ReflectedVar.h"
#include "EditorTypes.h"
#include "ReflectedVarRenderer.h"

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

static std::vector<Reflect::Var> ReflVars;
static auto MyRootNode = std::make_unique<Editor::TreeNode>("my node 1");
static auto MyNode2 = std::make_unique<Editor::TreeNode>("my node 2");
static Editor::AssetBrowser s_AssetBrowser{};
static Editor::TreeNodeRenderer s_TreeNodeRenderer{};

void Start() {


    /*static bool BoolForProp{ false };
    PropCont.ExportProperty<bool>(BoolForProp, "my new bool property ");


    static std::string StrForProp{ "my string " };
    PropCont.ExportProperty(StrForProp, "my bool prop ");

    static int IntForProp{ 4 };
    PropCont.ExportProperty(IntForProp, "my bool prop ");

    static double DoubleForProp{ 9.42489 };
    PropCont.ExportProperty(DoubleForProp, "my bool prop ");

    static float FloatForProp{ 9.42489 };
    PropCont.ExportProperty(FloatForProp, "my bool prop ");
    Editor::Property<float> FloatProp{ FloatForProp, "my f prop" };

    static Editor::Vector2 Vec2ForProp{ 9.42489, 138.39 };
    PropCont.ExportProperty(Vec2ForProp, "my bool prop ");

    static Editor::Color4 ColForProp{ 1.0,0.0,1.0,1.0 };

    PropCont.ExportProperty(ColForProp, "my bool prop ");

    static int t_int = 4;
    static std::string t_str = "hed";
    static float t_fl = 3.5934;
    static float t_db = 34.5934;
    static Editor::Color4 t_col4{};
    static bool t_bool{};
    static Editor::Vector2 t_vec2{};
    
    ReflVars.push_back(Reflect::Var{ &t_int, Reflect::Type::IntType, "my int var" });
    ReflVars.push_back(Reflect::Var{ &t_str, Reflect::Type::StringType, "my string var" });
    ReflVars.push_back(Reflect::Var{ &t_col4, Reflect::Type::Color4Type, "my string var" });
    ReflVars.push_back(Reflect::Var{ &t_fl, Reflect::Type::FloatType, "my fl var" });
    ReflVars.push_back(Reflect::Var{ &t_db, Reflect::Type::DoubleType, "my db var" });
    ReflVars.push_back(Reflect::Var{ &t_bool, Reflect::Type::BoolType, "my db var" });
    ReflVars.push_back(Reflect::Var{ &t_vec2, Reflect::Type::Vec2Type, "my db var" });

    MyRootNode->AddChild(std::move(MyNode2));
    MyRootNode->AddChild(std::make_unique < Editor::TreeNode>("my node 3"));
    MyRootNode->Children.back()->AddChild(std::make_unique < Editor::TreeNode>("my node 3"));



};

void ShowMyProperties()
{

    for (auto& Var : ReflVars)
    {
        Editor::RenderVar(Var);
    }

}

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
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED);
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+SDL_Renderer example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1000, window_flags);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return 0;
    }

    //SDL_RendererInfo info;
    //SDL_GetRendererInfo(renderer, &info);
    //SDL_Log("Current SDL_Renderer: %s", info.name);

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
    bool show_demo_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //start func call
    Start();

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {

            ImGuiWindowFlags PropertyPanelFlags = ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
            ImGui::Begin("Property inspector", (bool*)0, PropertyPanelFlags);
            ShowMyProperties();
            ImGui::End();



            ImGuiWindowFlags TreePanelFlags = ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

            ImGui::Begin("Tree panel", (bool*)0, TreePanelFlags);


            bool AddNodeButtonClicked = ImGui::Button("[+] Add Tree Node");
            ImGui::SameLine();
            bool RemoveNodeButtonClicked = ImGui::Button("[-] Remove Tree Node");

            s_TreeNodeRenderer.Render(*MyRootNode);

            if (AddNodeButtonClicked and s_TreeNodeRenderer.SelectedNode != nullptr) {
                s_TreeNodeRenderer.SelectedNode->AddChild(std::make_unique<Editor::TreeNode>("Added tree node"));
                s_TreeNodeRenderer.SelectedNode->ImGuiFlags |= ImGuiTreeNodeFlags_DefaultOpen;
            }

            if (RemoveNodeButtonClicked && s_TreeNodeRenderer.SelectedNode != nullptr) {

                s_TreeNodeRenderer.SelectedNode->RemoveFromParent();
                s_TreeNodeRenderer.SelectedNode = nullptr;
            }


            ImGui::End();


            ImGuiWindowFlags AssetBrowserFlags = ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
            ImGui::Begin("Asset Browser", (bool*)0, AssetBrowserFlags);
            s_AssetBrowser.Update();
            ImGui::End();



        }


        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
*/
