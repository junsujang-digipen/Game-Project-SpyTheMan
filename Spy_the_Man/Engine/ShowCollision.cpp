/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ShowCollision.cpp
Project: CS230
Author: Kevin Wright
Creation date: 4/29/2021
-----------------------------------------------------------------*/
#include "ShowCollision.h"
#include "Engine.h"

ShowCollision::ShowCollision(PM::InputKey::Keyboard keyToUse) : Component("ShowCollision"),
enabled(false), showCollisionKey(keyToUse) {}

void ShowCollision::Update(double) {
#ifdef _DEBUG
	if (showCollisionKey.IsKeyReleased() == true) {
		enabled = !enabled;
		Engine::GetLogger().LogDebug({ "ShowCollision is " + (enabled == true) ? "on" : "off" });
	}
#endif
}

bool ShowCollision::IsEnabled() {
	return enabled;
}