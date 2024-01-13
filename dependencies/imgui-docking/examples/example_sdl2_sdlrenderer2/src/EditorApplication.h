#pragma once
#include "Application.h"
#include <memory>
#include "Property.h"
#include "PropertyContainer.h"
#include "TreeNode.h"
#include "AssetBrowser.h"
#include "TreeNodeRenderer.h"
#include "ReflectedVar.h"
#include "EditorTypes.h"
#include "ReflectedVarRenderer.h"
#include "SDL.h"
#include "imgui.h"
#include <stdio.h>
#include "imgui_impl_sdlrenderer2.h"
#include "imgui_impl_sdl2.h"

class EditorApplication : public Application
{

public:
    EditorApplication() {};
    ~EditorApplication();

private:
    SDL_Window* window{ nullptr };
    SDL_Renderer* renderer{ nullptr };
    ImGuiIO* io{ nullptr };

private:
    void Startup();
    void Loop();
    void InputLoop();
    

    void ShowMyProperties();

    // Our state
    bool show_demo_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    std::vector<Reflect::Var> ReflVars;
    std::unique_ptr<Editor::TreeNode> MyRootNode = std::make_unique<Editor::TreeNode>("my node 1");
    std::unique_ptr<Editor::TreeNode> MyNode2 = std::make_unique<Editor::TreeNode>("my node 2");
    Editor::AssetBrowser s_AssetBrowser{};
    Editor::TreeNodeRenderer s_TreeNodeRenderer{};
};

