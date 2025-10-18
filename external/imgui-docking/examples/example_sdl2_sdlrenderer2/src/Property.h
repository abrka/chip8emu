//#pragma once
//#include "imgui.h"
//#include <string>
//#include <misc/cpp/imgui_stdlib.h>
//#include <iostream>
//#include <crossguid/guid.hpp>
//#include <cassert>
//
//
//namespace Editor {
//
//    class IProperty {
//    public:
//        virtual void Render() {};
//    };
//
//
//    template <typename T>
//    class Property : public IProperty {
//    public:
//        Property(T& _Property, std::string _PropertyName) : m_Property(_Property), m_PropertyName(_PropertyName) { };
//
//        //this function only gets called if no specialization is found aka the type cannot be rendered
//        void Render() override {
//            assert(false && "this property type cannot be rendered");
//        };
//
//    public:
//        T& m_Property;
//        std::string m_PropertyName;
//        xg::Guid UUID = xg::newGuid();
//        //we need to use a unique id for imgui widgets otherwise it will crash
//        inline std::string GetImGuiIDFromName() {
//            return (m_PropertyName + "##" + UUID.str());
//        }
//
//    };
//
//    template<>
//    void Property<int>::Render()
//    {
//        ImGui::InputInt(GetImGuiIDFromName().c_str(), &m_Property);
//    }
//
//    template<>
//    void Property<float>::Render()
//    {
//
//        ImGui::DragFloat(GetImGuiIDFromName().c_str(), &m_Property);
//    }
//
//    template<>
//    void Property<double>::Render()
//    {
//        static float m_PropertyAsFloat = static_cast<float>(m_Property);
//        ImGui::DragFloat(GetImGuiIDFromName().c_str(), &m_PropertyAsFloat);
//    }
//
//    template<>
//    void Property<std::string>::Render()
//    {
//        ImGui::InputText(GetImGuiIDFromName().c_str(), &m_Property);
//    }
//
//    template<>
//    void Property<bool>::Render()
//    {
//
//        ImGui::Checkbox(GetImGuiIDFromName().c_str(), &m_Property);
//    }
//
//    template<>
//    void Property<Color4>::Render()
//    {
//        static float Color[4] = { m_Property.r, m_Property.g, m_Property.b ,m_Property.a };
//        ImGui::ColorEdit4(GetImGuiIDFromName().c_str(), Color);
//    }
//
//    template<>
//    void Property<Vector2>::Render()
//    {
//        static float Vector[2] = { m_Property.x, m_Property.y };
//        ImGui::DragFloat2(GetImGuiIDFromName().c_str(), Vector);
//
//    }
//
//
//};
//
