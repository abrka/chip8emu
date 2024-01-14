#include "TreeNodeRenderer.h"
#include "TreeNode.h"
#include <iostream>
#include "imgui.h"

//accepts: the root node
void Editor::TreeNodeRenderer::Render(TreeNode& Node)
{

    if (SelectedNode == &Node) {
        Node.ImGuiFlags |= ImGuiTreeNodeFlags_Selected;
    }
    else {
        Node.ImGuiFlags &= ~ImGuiTreeNodeFlags_Selected;
    }

    bool IsNodeOpen = ImGui::TreeNodeEx(Node.GetImguiID().c_str(), Node.ImGuiFlags);

    if (ImGui::IsItemClicked()) {
        SelectedNode = &Node;
    }

    if (IsNodeOpen) {

        for (auto& Child : Node.Children)
        {
            Render(*Child);
        }

        ImGui::TreePop();
    }


}
