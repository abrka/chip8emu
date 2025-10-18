/*/#pragma once
#include "Property.h"
#include <vector>
#include <memory>
#include <iostream>
#include <any>

namespace Editor {

    class PropertyContainer {
    public:
        std::vector<std::unique_ptr<IProperty>> Properties;

    public:
        template <typename PropType>
        void ExportProperty(PropType& p_Property, std::string p_PropertyName) {
            std::unique_ptr<IProperty> AddedProp = std::make_unique<Property<PropType>>(p_Property, p_PropertyName);
            Properties.push_back(std::move(AddedProp));

        };

        void RenderAllProperties() {

            for (auto& Prop : Properties)
            {
                Prop->Render();
                
            }
        };
    };

    

}
*/
