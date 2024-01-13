#include "ReflectedVarRenderer.h"
#include <cassert>
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <sstream>
#include "EditorTypes.h"
#include <iostream>


std::string Editor::GetImGuiIDFromName(const Reflect::Var& _Var)
{

    std::ostringstream Temp;
    Temp << &_Var;
    return _Var.Name + "##" + Temp.str();

}

void Editor::RenderVar(const Reflect::Var& _Var)
{
    
    switch (_Var.VarType)
    {
    case Reflect::Type::None:
    {
        assert(false && "the var type is not set.");
        break;
    }

    case Reflect::Type::IntType:
    {
        ImGui::InputInt(GetImGuiIDFromName(_Var).c_str(), (int*)_Var.Data);
        break;
    }

    case Reflect::Type::FloatType:
    {
        ImGui::DragFloat(GetImGuiIDFromName(_Var).c_str(), (float*)_Var.Data);
        break;
    }

    case Reflect::Type::DoubleType:
    {
        static float* m_PropertyAsFloat = static_cast<float*>(_Var.Data);
        ImGui::DragFloat(GetImGuiIDFromName(_Var).c_str(), m_PropertyAsFloat);
        break;
    }

    case Reflect::Type::StringType:
    {
        ImGui::InputText(GetImGuiIDFromName(_Var).c_str(), (std::string*)_Var.Data);
        break;
    }

    case Reflect::Type::BoolType:
    {
        ImGui::Checkbox(GetImGuiIDFromName(_Var).c_str(), (bool*)_Var.Data);
        break;
    }

    case Reflect::Type::Color4Type:
    {
        Editor::Color4* DataAsCol4 = static_cast<Editor::Color4*>(_Var.Data);
        static float Color[4] = { DataAsCol4->r, DataAsCol4->g, DataAsCol4->b ,DataAsCol4->a };
        ImGui::ColorEdit4(GetImGuiIDFromName(_Var).c_str(), Color);
        break;
    }

    case Reflect::Type::Vec2Type:
    {
        Editor::Vector2* DataAsVec2 = static_cast<Editor::Vector2*>(_Var.Data);
        static float Vector[2] = { DataAsVec2->x, DataAsVec2->y };
        ImGui::DragFloat2(GetImGuiIDFromName(_Var).c_str(), Vector);
        break;
    }

    default:
    {
        assert(false && "this var type cannot be rendered");
        break;
    }
    }
}

