/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: newLevel.cpp
Project: Spy the Man
Author: 
Creation date: 5/2/2022
-----------------------------------------------------------------*/
#include "LoadLevel.h"

#include "../../Engine/Engine.h"
#include "../WorldGuardLevel.h"
#include "../../Engine/World.h"
#include "../Player.h"
#include "../Engine/Sequence/SequenceManager.h"
#include "../Engine/SoundManager.h"

#include "../UI/Phone.h"

#include "../Object/Clothes.h"
#include "../Object/SideDoor.h"

#include "../Engine/Effect/EffectManager.h"
#include "../Engine/ShowCollision.h"

#include "../Game/Object/Action/ActionManager.h"
#include "../Game/UI/HelpText.h"
#include "../Engine/Wave.h"

#include "../ValidManager.h"

#include "../ClearManager.h"
#include "../Screens.h"
#include "../GSButtonManager.h"
#include "../Master_AI.h"
#include "../Engine/IOManager/ObjectData.h"
LoadLevel::LoadLevel()
{
	
}

void LoadLevel::Load()
{
	BackgroundBackPtr = Engine::GetTextureManager().Load("assets/Background/cityBackgroundBack.png");
	BackgroundMiddlePtr = Engine::GetTextureManager().Load("assets/Background/cityBackgroundMiddle.png");
	BackgroundFrontPtr = Engine::GetTextureManager().Load("assets/Background/cityBackgroundFront.png");
	AddGSComponent(new Master_AI());
	AddGSComponent(new PM::SequenceManager{});
	AddGSComponent(new WorldGuardLevel{});
	AddGSComponent(new PM::UI::UIManager());
	AddGSComponent(new HelpText());
	AddGSComponent(new ValidManager());
	PM::LevelData* data = Engine::GetLevelIOManager().ReadLevel();
	world = new PM::World( data->GetChunkSize());
	AddGSComponent(world);
	
	AddGSComponent(new ShowCollision(PM::InputKey::Keyboard::Tilde));
	AddGSComponent(new ClearManager());
	
	LoadObj(data);
	LoadRoom(data);

	AddGSComponent(new EffectManager());
	AddGSComponent(new ActionManager());
	Engine::GetGSComponent<ActionManager>()->SetAction(data);
	AddGSComponent(new Phone());

	AddGSComponent(new GSButtonManager());
	Engine::GetSoundManager().PlayBGM("BGM0");


	playerptr = dynamic_cast<Player*>(world->GetPlayerPtr());
	Engine::GetCamera().bindTarget(playerptr);
	Engine::GetCamera().setPosition(playerptr->GetPosition());
	Engine::GetCamera().setExtend(glm::mat2{ { -5000,-2000 },  {15000 , 5000} });
	Engine::GetFont().Load("fonts/ABeeZee-Regular.ttf", 50);
	delete data;
	openingAni = true;
}

void LoadLevel::LoadObj(PM::LevelData* data)
{
	auto objlist = data->GetobjData();
	for (auto objdata : objlist)
	{
		if(	objdata->GetEntityType() == EntityType::Player)
		{
			world->Add(objdata->DataToObj());
			break;
		}
	}

	for (auto objdata : objlist)
	{
		if (objdata->GetEntityType() != EntityType::Player) 
		{
			PM::GameObject* obj = objdata->DataToObj();
			if (obj == nullptr)
			{
				Engine::GetLogger().LogError("data can't change the object");
				continue;
			}
			world->Add(obj);
		}
	}

}

void LoadLevel::LoadRoom(PM::LevelData* data)
{
	auto roomlist = data->GetRoomData();
	for (auto roomdata : roomlist)
	{
		PM::GameObject* L = nullptr;
		PM::GameObject* R = nullptr;
		if(roomdata->Left != "empty")
		{
			L = world->GetObject(roomdata->Left);
			if (L == nullptr)
			{
				Engine::GetLogger().LogError("data can't Find the object");
			}
		}
		if (roomdata->Right != "empty")
		{
			R = world->GetObject(roomdata->Right);
			if(R == nullptr)
			{
				Engine::GetLogger().LogError("data can't Find the object");
			}
		}
		world->AddRoom(new PM::Room(roomdata->Name, roomdata->position, roomdata->size, L, R));
	}

}


void LoadLevel::Update(double dt)
{
	if (openingAni == true)
	{
		if (OpeningRectPos.x < -1700)
		{
			openingAni = false;
		}
		else
		{

			OpeningRectPos.x -= Engine::GetWindow().GetSize().x * 2.3 * dt;
		}
	}
	dt *= Engine::GetGSComponent<PM::World>()->GetdtMagnification();
	UpdateGSComponents(dt);

#ifdef _DEBUG
	if(Engine::GetInput().IsKeyReleased(PM::InputKey::Keyboard::V))
	world->SetCanUseButton(!world->GetCanUseButton());
#endif

	if (Engine::GetInput().IsKeyReleased(PM::InputKey::Keyboard::Escape) && playerptr->IsDead() == false)
	{
		Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::LevelSelect));
	}
	if (playerptr->IsDestroy() || Engine::GetInput().IsKeyReleased(PM::InputKey::Keyboard::R) && playerptr->IsDead() == false)
	{
		Engine::GetGameStateManager().ReloadState();
	}
#ifdef _DEBUG
	if (Engine::GetInput().RightMouseIsReleased())
	{
		new PM::Wave(Engine::GetInput().GetWorldMousePos(), 8., 3);
	}
	if (Engine::GetInput().IsKeyDown(PM::InputKey::Keyboard::L))
	{
		world->SetIsClear(true);
	}
#endif
	
}

void LoadLevel::Unload()
{
	ClearGSComponent();
	playerptr = nullptr;
	//camera unbinding
	Engine::GetCamera().unBinding();
	Engine::GetSoundManager().Pause();
	Engine::GetSoundManager().setStop(false);
}

void LoadLevel::Draw()
{
	Engine::GetWindow().SetBackground(0.184f, 0.694f, 0.992f, 1.f); 
	Engine::GetWindow().Clear();
	math::TransformMatrix cam;
	cam.matrix = PM::getCameraMat();


	glm::vec2 buildingSize = { world->GetChunkSize().x * ROOMXSIZE , world->GetChunkSize().y * ROOMYSIZE };
	glm::mat3 transform1 = { {1, 0, 0},{0, 1, 0},{Engine::GetCamera().getCameraOrigin().x * 3 / 4, Engine::GetCamera().getCameraOrigin().y * 10 / 100, 1} };
	glm::mat3 transform2 = { {1, 0, 0},{0, 1, 0},{Engine::GetCamera().getCameraOrigin().x * 2 / 4, Engine::GetCamera().getCameraOrigin().y * 5 / 100, 1} };
	glm::mat3 transform3 = { {1, 0, 0},{0, 1, 0},{Engine::GetCamera().getCameraOrigin().x * 1 / 4, 0, 1} };
	BackgroundBackPtr->Draw(cam * transform1, { -5000, -50 }, { 20000 , 2000 });
	BackgroundMiddlePtr->Draw(cam * transform2, { -5000, -50 }, { 20000 , 2000 });
	BackgroundFrontPtr->Draw(cam * transform3, { -5000, -50 }, { 20000 , 2000 });
	PM::push_settings();
	PM::set_fill_color(100, 100, 100, 255);
	PM::apply_matrix(cam.matrix);
	PM::draw_rect(0, 0, buildingSize.x, buildingSize.y);
	PM::set_fill_color(150, 50, 50, 255);
	PM::draw_rect(-50000, -10000, 100000, 10000);
	PM::pop_settings();

	
	
	world->DrawAll(cam);

	GetGSComponent<EffectManager>()->DrawAll(cam);

	Engine::GetGSComponent<PM::UI::UIManager>()->Draw();

	glm::ivec2 winSize = Engine::GetWindow().GetSize();
	if (openingAni == true)
	{
		PM::push_settings();
		PM::apply_NDC();
		PM::set_fill_color(0, 0, 0, 255);
		PM::no_outline();
		PM::draw_rect(OpeningRectPos.x, OpeningRectPos.y, winSize.x + 100, winSize.y);
		PM::pop_settings();
	}
}

