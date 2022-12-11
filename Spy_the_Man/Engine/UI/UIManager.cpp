/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include "UIManager.h"
#include "Mouse.h"
#include "../Collision.h"
#include "../Engine.h"
#include "../World.h"
#include "../Camera.h"

glm::vec2 PM::UI::UIManager::sequenceButtonSize{ 60,60 };
void PM::UI::UIManager::Load()
{
	mouse = new UI::Mouse();
}

void PM::UI::UIManager::Update(double dt)
{
	for (pair<const int, UIEntity*>& ui : Interfaces)
	{
		if (ui.second->isValid() == true)
		{
			ui.second->Update(dt);
		}
		if(ui.second->ShouldDestroy())
		{
			destroyInterfaces.push_back(ui);
		}
	}
	mouse->Update(dt);
	CollideTest();
	for (pair<const int, UIEntity*> ui : destroyInterfaces)
	{
		if(mouse->GetCollidedObj() == ui.second)
		{
			mouse->SetCollidedObj(nullptr);
		}
		auto iter = find(Interfaces.begin(), Interfaces.end(), ui);
		Interfaces.erase(iter);
		delete ui.second;
	}
	destroyInterfaces.clear();
}

void PM::UI::UIManager::Draw()
{
	math::TransformMatrix cam;
	cam.matrix = { {2.f / Engine::GetWindow().GetSize().x, 0, 0}, {0, 2.f / Engine::GetWindow().GetSize().y, 0}, {-1.f, -1.f, 1.f} };
	for (pair<const int, UIEntity*>& ui : Interfaces)
	{
		if (ui.second->isValid() == true)
		{
			ui.second->Draw(cam);
		}
	}
	//mouse->Draw(cam);
}

void PM::UI::UIManager::CollideTest()
{
	mouse->SetCollidedObj(nullptr);
	if (Interfaces.size() != 0)
	{
		for (auto iter = (--Interfaces.end()); iter != (--Interfaces.begin()); iter--)
		{
			if (iter->second->isValid() == true)
			{
				Collision* collision = iter->second->GetGOComponent<Collision>();
				if (collision != nullptr)
				{
					iter->second->UpdateCollision(mouse);
				}
			}
		}
		UIEntity* col = mouse->GetCollidedObj();
		if (col != nullptr)
		{
			if (col->GetCollided() == false)
			{
				col->SetCollided(true);
				col->CollisionEnter(mouse);
				col->CollisionStay(mouse);
			}
			else
			{
				col->SetCollided(true);
				col->CollisionStay(mouse);
			}
		}
	}
}

void PM::UI::UIManager::Unload()
{
	delete mouse;
	for(pair<const int, UIEntity*> ui : Interfaces)
	{
		delete ui.second;
	}
	Interfaces.clear();
	destroyInterfaces.clear();
	mouse = nullptr;
}

void PM::UI::UIManager::AddUI(UIEntity* ui, UILayer layer)
{
	Interfaces.insert(make_pair(static_cast<int>(layer), ui));
}



