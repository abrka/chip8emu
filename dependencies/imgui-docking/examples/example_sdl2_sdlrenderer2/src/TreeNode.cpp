#include "TreeNode.h"
#include <assert.h>
#include <iostream>


std::string Editor::TreeNode::GetImguiID() const
{
    return std::string{ Name + "##" + reinterpret_cast<const char*>(this) };
}



void Editor::TreeNode::RemoveThisFromParentsChildren()
{
    Parent->Children.erase(
        std::remove_if(Parent->Children.begin(), Parent->Children.end(),
            [this](std::unique_ptr<Editor::TreeNode>& Ent) {           
                return Ent.get() == this;
            }), Parent->Children.end());
}

void Editor::TreeNode::RemoveFromParent()
{
    if (Parent == nullptr) {
        std::cout << "parent is null. returning from remove parent" << std::endl;
        return;
    }

    RemoveThisFromParentsChildren();
    Parent = nullptr;
}

void Editor::TreeNode::AddChild(std::unique_ptr<Editor::TreeNode>&& Child)
{
    assert(Child != nullptr && "Child is nullptr");
    assert(Child.get() != this && "cannot add this entity as a child of this entity");

    if (Child->Parent != nullptr) {
        Child->RemoveFromParent();
    }

    Child->Parent = this;
    Children.push_back(std::move(Child));


}
