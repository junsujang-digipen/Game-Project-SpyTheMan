/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: MainMenu.cpp
Project: CS200
Author: Dong-A Choi
Creation date: 10/12/2021
-----------------------------------------------------------------*/
#include "../Engine/Engine.h"	//GetGameStateManager GetSpriteFont
#include "../Engine/TransformMatrix.h"
#include "../Engine/ShowCollision.h"
#include "Screens.h"
#include "MainMenu.h"
#include "Player.h"
#include "../Engine/Poodle.h"
#include "../Engine/SoundManager.h"
#include "../Engine/Declaration.h"

MainMenu::MainMenu() 
{}

void MainMenu::Load() {
	Engine::GetFont().Load("fonts/ABeeZee-Regular.ttf", 50);
	AddGSComponent(new PM::UI::UIManager());
	BackgroundBackPtr = Engine::GetTextureManager().Load("assets/Background/cityBackgroundBack.png");
	BackgroundMiddlePtr = Engine::GetTextureManager().Load("assets/Background/cityBackgroundMiddle.png");
	BackgroundFrontPtr = Engine::GetTextureManager().Load("assets/Background/cityBackgroundFront.png");
	TitlePtr = Engine::GetTextureManager().Load("assets/Title.png");
	PreTitlePtr = Engine::GetTextureManager().Load("assets/TitleHuman.png");
	Engine::GetSoundManager().PlayBGM("Title");
	Engine::GetSoundManager().setBGMOffset("Title",0.f);
	glm::ivec2 winSize = Engine::GetWindow().GetSize();
	StartButton = new PM::UI::TextButton("Start", { winSize.x / 2 -125, 220 }, { 250, 140 }, "Start");
	CreditButton = new PM::UI::TextButton("Credit", { winSize.x / 2 - 125, 30 }, { 250, 140 }, "Credit");
	Engine::SetTitleSkip(false);
	StartButton->Setvalid(false);
	CreditButton->Setvalid(false);
	titleEnd = false;
	titleEndTime = 0.f;
	pretitleX = 3400.f;
	preTitleTime = 0.f;
	EndAni = false;
	EndRectPos = { 1600,0 };
}

void MainMenu::Update(double dt) {
	if (EndAni == true)
	{
		if(EndRectPos.x < 0)
		{
			EndRectPos.x = 0;
			Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::LevelSelect));
		}
		else
		{
			EndRectPos.x -= Engine::GetWindow().GetSize().x * 2.3 * dt;
		}
	}
	else
	{
		
		if ((Engine::GetInput().LeftMouseIsReleased() || pretitleX <= 0) && titleEnd == false)
		{
			titleEnd = true;
			//Engine::GetSoundManager().Pause();
			Engine::SetTitleSkip(true);

		}

		if (titleEnd == true)
		{
			titleEndTime += static_cast<float>(dt);
			backgroundPos = Engine::GetInput().GetDeviceMousePos();
			backgroundPos.y -= 700.;

			if (StartButton->IsReleased() == true)
			{
				//Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::LevelSelect));
				EndAni = true;
			}
			if(CreditButton->IsReleased() == true)
			{
				Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::Credit));
			}
			if (titleEndTime > 1.5f && titleEndTime < 2.f)
			{
				//Engine::GetSoundManager().PlayBGM("Title");
				//Engine::GetSoundManager().setBGMOffset("Title", 29.67);
				StartButton->Setvalid(true);
				CreditButton->Setvalid(true);
				titleEndTime = 3.f;
			}
		}
		else
		{
			backgroundPos.x -= 500. * dt;
			pretitleX -= static_cast<float>(500 * dt);
		}
	}
	if(Engine::GetInput().IsKeyDown(PM::InputKey::Keyboard::Num9))
	{
		Engine::GetLevelIOManager().ResetAllChapter();
	}

	UpdateGSComponents(dt);
	
}

void MainMenu::Unload() {
	ClearGSComponent();

	Engine::GetSoundManager().Pause();
	Engine::GetCamera().unBinding();
}

void MainMenu::Draw() {
	Engine::GetWindow().SetBackground(0.184f, 0.694f, 0.992f, 1.f);//0x3399DAFF
	Engine::GetWindow().Clear();

	glm::ivec2 winSize = Engine::GetWindow().GetSize();
	math::TransformMatrix cam;

	glm::mat3 NDC_{ {2.f / Engine::GetWindow().GetSize().x, 0, 0}, {0, 2.f / Engine::GetWindow().GetSize().y, 0}, {-1.f, -1.f, 1.f} };
	cam.matrix = NDC_;

	glm::mat3 transform1 = { {1, 0, 0},{0, 1, 0},{backgroundPos.x * 3 / 4, backgroundPos.y * 10 / 100, 1} };
	glm::mat3 transform2 = { {1, 0, 0},{0, 1, 0},{backgroundPos.x * 2 / 4, backgroundPos.y * 5 / 100, 1} };
	glm::mat3 transform3 = { {1, 0, 0},{0, 1, 0},{backgroundPos.x * 1 / 4, 0, 1} };

	BackgroundBackPtr->Draw(cam * transform3, { -5000, -250 }, { 20000 , 1800 });
	BackgroundMiddlePtr->Draw(cam * transform2, { -5000, -250 }, { 20000 , 1800 });
	BackgroundFrontPtr->Draw(cam * transform1, { -5000, -250 }, { 20000 , 1800 });

	glm::ivec2 titlepos = (winSize / 2) - (TITLESIZE / 2) + glm::ivec2{0,150};

	if (titleEnd == false)
	{
		PreTitlePtr->Draw(NDC_, titlepos + glm::ivec2{ pretitleX,0 }, TITLESIZE);
	}

	PM::push_settings();
	PM::apply_NDC();
	PM::use_shader("Title");
	PM::set_fill_color(0, 0, 0);
	PM::draw_rect(0, 0, winSize.x, winSize.y);
	PM::pop_settings();
	

	Engine::GetGSComponent<PM::UI::UIManager>()->Draw();
	
	if (titleEnd == true)
	{
		TitlePtr->Draw(NDC_, titlepos, TITLESIZE);
		PM::push_settings();
		PM::apply_NDC();
		PM::set_fill_color(255, 255, 255, 255 - (titleEndTime * 255 * 1.5));
		PM::draw_rect(0, 0, winSize.x, winSize.y);
		PM::pop_settings();
	}
	if(EndAni == true)
	{
		PM::push_settings();
		PM::apply_NDC();
		PM::set_fill_color(0, 0, 0, 255);
		PM::no_outline();
		PM::draw_rect(EndRectPos.x, EndRectPos.y, winSize.x + 300, winSize.y);
		PM::pop_settings();
	}
}
