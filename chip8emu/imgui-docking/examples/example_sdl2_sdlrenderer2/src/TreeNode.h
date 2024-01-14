#pragma once
#include <vector>
#include <string>
#include <memory>
#include <string_view>
#include "imgui.h"


namespace Editor {


    class TreeNode {

    public:
        TreeNode(std::string_view _Name) : Name(_Name) {};
        std::string Name{};
        TreeNode* Parent = nullptr;
        std::vector < std::unique_ptr<TreeNode>> Children;

        ImGuiTreeNodeFlags ImGuiFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        std::string GetImguiID() const;

        void RemoveFromParent();
        void AddChild(std::unique_ptr<Editor::TreeNode>&& Child);

    private:
        void RemoveThisFromParentsChildren();

    };


}
