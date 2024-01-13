#pragma once
#include <filesystem>

namespace Editor {
    class AssetBrowser
    {
    public:
        AssetBrowser();
        void Update();
    private:
        void DrawFiles();
        const std::filesystem::path StartingPath{ std::filesystem::current_path() };
        std::filesystem::path SelectedPath{ StartingPath };
    };
}
