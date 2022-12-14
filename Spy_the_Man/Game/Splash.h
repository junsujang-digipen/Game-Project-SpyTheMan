/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Splash.h
Project: CS230
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#pragma once
#include "..\Engine\GameState.h"

namespace PM {
	class Texture;
}

class Splash : public PM::GameState {
public:
	Splash();
	void Load() override;
	void Update(double dt) override;
	void Unload() override;
	void Draw() override;

	std::string GetName() override { return "Splash"; }
private:
	double timer{0.};
	const int TIME = 3;
	const int DISPLAY_TIME = 6;
	glm::dvec2 pos = { 0., -1200. };
};

