#pragma once


namespace Editor {

    class TreeNode;

    class TreeNodeRenderer {
    public:
        TreeNode* SelectedNode = nullptr;
        void Render(TreeNode& Node);
    };
}
