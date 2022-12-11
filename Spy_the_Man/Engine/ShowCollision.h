/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ShowCollision.h
Project: CS230
Author: Kevin Wright
Creation date: 4/29/2021
-----------------------------------------------------------------*/
#pragma once
#include "Input.h"
#include "Component.h"

class ShowCollision : public PM::Component {
public:
    ShowCollision(PM::InputKey::Keyboard keyToUse);
    void Update(double dt) override;
    bool IsEnabled();
private:
    bool enabled;
    PM::InputKey showCollisionKey;
};