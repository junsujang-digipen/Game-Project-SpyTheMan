/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: MainMenu.h
Project: CS230 , CS200
Author: Kevin Wright && Dong-A Choi
Creation date: 2/16/2021 , 10/12/2021
-----------------------------------------------------------------*/
#pragma once
#include <string>
#include "..\Engine\GameState.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "..\Engine\Input.h"
#include "..\Engine\GameObjectManager.h"
#include "../Engine/Texture.h"
#include "Building.h"
#include "../Engine/World.h"
#include "../Engine/UI/Button.h"
#include "../Engine/Font.h"
#include "../Engine/Sprite.h"

class Player;

class MainMenu : public PM::GameState {
public:
	MainMenu();
	void Load() override;
	void Update(double dt) override;
	void Unload() override;
	void Draw() override;

	std::string GetName() override { return "MainMenu"; }
private:

	PM::UI::Button* StartButton{nullptr};
	PM::UI::Button* CreditButton{ nullptr };
	bool titleEnd = false;
	float titleEndTime = 0.f;
	glm::dvec2 backgroundPos{0,0};
	const glm::ivec2 TITLESIZE{ 960,460 };
	float pretitleX = 3400.f;
	float preTitleTime{0};

	bool EndAni = false;
	glm::dvec2 EndRectPos{1600,0};
	
	PM::Texture* TitlePtr{ nullptr };
	PM::Texture* PreTitlePtr{ nullptr };
	PM::Texture* BackgroundBackPtr{ nullptr };
	PM::Texture* BackgroundMiddlePtr{ nullptr };
	PM::Texture* BackgroundFrontPtr{ nullptr };
};