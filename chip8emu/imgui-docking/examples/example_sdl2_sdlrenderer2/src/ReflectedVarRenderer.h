#pragma once
#include "ReflectedVar.h"
#include <string>


namespace Editor {


    //we need to use a unique id for imgui widgets otherwise it will crash
    std::string GetImGuiIDFromName(const Reflect::Var& _Var);

    void RenderVar(const Reflect::Var& _Var);
}
