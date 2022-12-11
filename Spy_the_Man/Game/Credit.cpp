/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Credit.cpp
Author: Dong-A Choi
-----------------------------------------------------------------*/
#include <vector>


#include "Credit.h"
#include "Screens.h"
#include "../Engine/Engine.h"

Credit::Credit() : now{0}, totalLine(0.f)
{
}

void Credit::Load()
{
	Engine::GetFont().Load("fonts/ABeeZee-Regular.ttf", 25);
	
	Engine::GetCamera().setCameraHeight(static_cast<float>(Engine::GetWindow().GetSize().y));

	AddGSComponent(new PM::UI::UIManager());

	std::string in_line;
	std::ifstream in("Credit.txt");
	while (std::getline(in, in_line))
	{
		totalLine -= 40.f;
		txtMap[totalLine] = in_line;
		now = 0;
		//pos.y += 20;
	}
}

void Credit::Draw()
{
	Engine::GetWindow().SetBackground(0.f, 0., 0., 1.f);
	Engine::GetWindow().Clear();
				
	//애초에 푸쉬백을 2초에 한번씩 해주면 되는구나!
	for (auto a : txtMap) {
		Engine::GetFont().Draw(a.second, pos.x, a.first, 1.f, true, PM::AlignType::center, { 1.f,1.f,1.f });
	}
	Engine::GetGSComponent<PM::UI::UIManager>()->Draw();
}

void Credit::Update(double dt)
{
	UpdateGSComponents(dt);

	glm::vec2 halfWinSize = Engine::GetWindow().GetSize() / 2;

	Engine::GetCamera().LockAr(true);
	//Engine::GetCamera().updateCameraHeight(1);
	now -= dt*60;
	glm::vec2 cameraPosition = { -halfWinSize.x,-halfWinSize.y + now };
	Engine::GetCamera().setPosition(cameraPosition);

	if (Engine::GetInput().IsKeyReleased(PM::InputKey::Keyboard::Escape))
	{
		Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::MainMenuTemp));
	}
}

void Credit::Unload()
{
	ClearGSComponent();
	Engine::GetCamera().LockAr(false);
}
