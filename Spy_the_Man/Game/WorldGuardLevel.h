/*--------------------------------------------------------------
File Name: WorldGuardLevel.cpp
Project: Spy The Man
Author: Junsu Jang
Creation date: 12/6/2021
-----------------------------------------------------------------*/
#pragma once
#include "../Engine/Component.h"

class WorldGuardLevel:public PM::Component {
public:
	WorldGuardLevel() = default;
	double Level{ 1. };
};