/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Splash.cpp
Project: CS230
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#include <filesystem>
#include "../Engine/Engine.h"	//GetGameStateManager	GetTextureManager
#include "../Engine/TransformMatrix.h"
#include "Screens.h"
#include "Splash.h"

Splash::Splash() {}

void Splash::Load() {

#ifdef _DEBUG
	Engine::Instance().GetGameStateManager().SetNextState(static_cast<int>(Screens::MainMenuTemp));
#endif
	

	timer = 0;
	PM::Image("assets/Splash.png");
	PM::Image("assets/PM_LOGO.png");
}

void Splash::Update(double dt) {
	timer += dt;
	if (timer >= DISPLAY_TIME) {
		Engine::Instance().GetGameStateManager().SetNextState(static_cast<int>(Screens::MainMenuTemp));
	}
	if (timer > TIME)
	{
		pos.y += dt * 500.;
		if(pos.y >= 0)
		{
			pos.y = 0;
		}
	}
}

void Splash::Unload() {

	ClearGSComponent();
}

void Splash::Draw() {
	Engine::GetWindow().SetBackground(1.f,1.f,1.f,1.f);
	Engine::GetWindow().Clear();

	glm::ivec2 winSize = Engine::GetWindow().GetSize();
	glm::mat3 digipen = { {2.f / winSize.x, 0, 0}, {0, 2.f / winSize.y, 0}, {-1.f, -1.f, 1.f} };
	
	if (timer <= TIME) {
		PM::draw_image("assets/Splash.png", digipen, { (winSize.x - 1024) / 2, (winSize.y - 338) / 2 }, { 1024, 338 });
	}
	else {
		PM::push_settings();
		PM::use_shader("Hologram");
		PM::apply_NDC();
		PM::set_fill_color(45, 61, 131, 100);
		PM::draw_rect(0, 0, Engine::GetWindow().GetSize().x, Engine::GetWindow().GetSize().y);
		PM::pop_settings();
		/*PM::push_settings();
		PM::apply_NDC();
		PM::no_outline();
		PM::set_fill_color(255, 85, 0, 255);
		PM::draw_rect(0,pos.y, 1600, 900);
		PM::pop_settings();
		*/PM::draw_image("assets/PM_LOGO.png", digipen, { (winSize.x - 512) / 2, (winSize.y - 384) / 2 }, { 512, 384 });
	}
	
}