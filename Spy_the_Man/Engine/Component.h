/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Component.h
Project: CS230
Author: Kevin Wright
Creation date: 2/16/2021
-----------------------------------------------------------------*/

#pragma once
#include <string>

namespace PM {
    class ComponentData;
		 
    class Component {
    private:
        bool isValid{ true };
        std::string name{ "EmptyComponent" };
    public:
        Component() = default;
    	Component(std::string n, bool valid = true) : name(n), isValid(valid) {}
        virtual ~Component() {};
        virtual void Update(double) {};
        virtual ComponentData* MakeDataFile() const { return nullptr; }
        virtual void setIsValid(bool tf) { isValid = tf; }
        bool getIsValid() const { return isValid; }
    };
}