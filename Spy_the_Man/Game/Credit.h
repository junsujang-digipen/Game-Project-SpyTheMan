/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Credit.h
Author: Dong-A Choi
-----------------------------------------------------------------*/
#pragma once
#include "../Engine/GameState.h"
#include <map>
class Credit : public PM::GameState
{
public:
	Credit();
	void Load() override;
	void Draw() override;
	void Update(double dt) override;
	void Unload() override;

	std::string GetName() override { return "Credit"; }
private:
	double now;
	float totalLine;
	glm::vec2 pos{0,0};
	std::map<float, std::string> txtMap;

};
