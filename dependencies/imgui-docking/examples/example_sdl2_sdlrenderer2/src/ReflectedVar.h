#pragma once
#include <string>

namespace Reflect {
    enum class Type {
        None = 0,
        IntType,
        FloatType,
        DoubleType,
        StringType,
        BoolType,
        Vec2Type,
        Color4Type

    };
    class Var
    {
    public:
        //the passed _Type is not checked. Wrong types will yield undefined behaviour
        Var(void* _Data, Type _Type, std::string _Name): Data(_Data), VarType(_Type), Name(_Name) {};

        void* Data = nullptr;
        Type VarType = Type::None;
        std::string Name{};

    };
}
