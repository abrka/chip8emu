#include "AssetBrowser.h"
#include <filesystem>
#include <iostream>
#include "imgui.h"

Editor::AssetBrowser::AssetBrowser()
{

}


void Editor::AssetBrowser::DrawFiles()
{
    static std::filesystem::path CurrentPath{ StartingPath };
    

    if (ImGui::Button("<-") and CurrentPath.has_parent_path() and CurrentPath != StartingPath) {
        CurrentPath = CurrentPath.parent_path();
    }

    for (auto& dir_entry : std::filesystem::directory_iterator{ CurrentPath }) {

     
        bool FileNodeOpen = ImGui::Selectable(dir_entry.path().filename().string().c_str(), SelectedPath == dir_entry.path());

        if (FileNodeOpen) {
            SelectedPath = dir_entry.path();
        }
        if (FileNodeOpen and dir_entry.is_directory()) {
            CurrentPath = dir_entry.path();
            
        }


    }
    

}


void Editor::AssetBrowser::Update()
{
    DrawFiles();
}
