/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: SkillChoose.h
Project: GAM200
Author: Dong-A Choi
-----------------------------------------------------------------*/
#pragma once
#include <string>
#include "..\Engine\GameState.h"
#include "..\Engine\Skill\SkillManager.h"
#include "..\Engine\Texture.h"
#include "../Engine/UI/Button.h"

class SkillChoose : public PM::GameState {
public:
	SkillChoose();
	void Load() override;
	void Update(double dt) override;
	void Unload() override;
	void Draw() override;
	std::string GetName() override { return "SkillChoose"; }

private:
	bool loading;
	double yettime = 0;
	double noptime = 0;

	PM::UI::Button* mainmenu;
	PM::UI::Button* finish;
	PM::UI::Button* decide;

	SkillType currskill;

	//skillbuttons
	PM::UI::Button* drone;
	PM::UI::Button* dronesound;
	PM::UI::Button* chameleon;
	PM::UI::Button* invisible;
	PM::UI::Button* visualize;
	PM::UI::Button* silentrun;
	PM::UI::Button* assassinate;
	PM::UI::Button* slowtrap;
	PM::UI::Button* detect;
	PM::UI::Button* push;
	PM::UI::Button* dash;
	PM::UI::Button* flashbomb;
};