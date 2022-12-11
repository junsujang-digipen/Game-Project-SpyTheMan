/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include "Tuto.h"
#include "../../Engine/Engine.h"
#include "../WorldGuardLevel.h"
#include "../../Engine/World.h"
#include "../Player.h"
#include "../Engine/Sequence/SequenceManager.h"
#include "../Engine/SoundManager.h"

#include "../UI/Phone.h"

#include "../Object/Clothes.h"
#include "../Object/Hide_Obj.h"
#include "../Object/SideDoor.h"
#include "../Object/Wall.h"
#include "../Object/Stair.h"


#include "../Basic_NPC.h"
#include "../Task_AI_basePosition.h"
#include "../Task_AI_pointToPoint.h"
#include "../Engine/Effect/EffectManager.h"
#include "../Engine/ShowCollision.h"

#include "../Game/Object/Action/ActionManager.h"
#include "../Game/UI/HelpText.h"
#include "../Object/Vent.h"
#include "../Object/Key.h"
#include "../Lazer.h"

#include "../ValidManager.h"
#include "../GSButtonManager.h"
#include "../Object/Goal.h"
#include "../Object/Switch.h"
#include "../ClearManager.h"
#include "../Master_AI.h"
#include "../Screens.h"

Tuto::Tuto()
{
	
}

void Tuto::Load()
{
	AddGSComponent(new Master_AI());
	AddGSComponent(new PM::SequenceManager{});
	AddGSComponent(new WorldGuardLevel{});
	AddGSComponent(new PM::UI::UIManager());
	AddGSComponent(new HelpText());
	AddGSComponent(new ValidManager());
	world = new PM::World({ 4,2 });
	AddGSComponent(world);
	AddGSComponent(new ShowCollision(PM::InputKey::Keyboard::Tilde));
	AddGSComponent(new ClearManager());
	playerptr = new Player({  200,    WALLTHICKNESS });
	Engine::GetCamera().bindTarget(playerptr);
	Engine::GetCamera().setPosition(playerptr->GetPosition());
	Engine::GetCamera().setExtend(glm::mat2{ { -5000,-2000 },  {15000 , 5000} });
	Engine::GetFont().Load("fonts/ABeeZee-Regular.ttf", 50);
	world->Add(playerptr);
	MapLoad();

	AddGSComponent(new EffectManager());
	AddGSComponent(new ActionManager());
	AddGSComponent(new Phone());
	AddGSComponent(new GSButtonManager());
	Engine::GetSoundManager().PlayBGM("BGM0");
}

void Tuto::Update(double dt)
{
	dt *= Engine::GetGSComponent<PM::World>()->GetdtMagnification();
	UpdateGSComponents(dt);

#ifdef _DEBUG
	if(Engine::GetInput().IsKeyReleased(PM::InputKey::Keyboard::V))
	world->SetCanUseButton(!world->GetCanUseButton());
#endif

	if (playerptr->IsDestroy())
	{
		Engine::GetGameStateManager().ReloadState();
	}
	if (Engine::GetInput().IsKeyReleased(PM::InputKey::Keyboard::R) && playerptr->IsDead() == false)
	{
		Engine::GetGameStateManager().ReloadState();
	}
	if (Engine::GetInput().IsKeyReleased(PM::InputKey::Keyboard::Escape) && playerptr->IsDead() == false)
	{
		Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::LevelSelect));
	}
}

void Tuto::Unload()
{
	ClearGSComponent();
	playerptr = nullptr;
	//camera unbinding
	Engine::GetCamera().unBinding();
	Engine::GetSoundManager().Pause();
	Engine::GetSoundManager().setStop(false);
}

void Tuto::Draw()
{
	Engine::GetWindow().SetBackground(1.f, 1.f, 1.f, 1.f);
	Engine::GetWindow().Clear();
	math::TransformMatrix cam;
	cam.matrix = PM::getCameraMat();

	glm::vec2 buildingSize = { world->GetChunkSize().x * ROOMXSIZE , world->GetChunkSize().y * ROOMYSIZE };
	PM::push_settings();
	PM::apply_matrix(cam.matrix);
	PM::set_fill_color(150, 50, 50, 255);
	PM::draw_rect(-50000, -10000, 100000, 10000 + buildingSize.y - ROOMYSIZE);
	PM::set_fill_color(PM::Room::BUILDINGCOLOR.r, PM::Room::BUILDINGCOLOR.g, PM::Room::BUILDINGCOLOR.b, 255);
	PM::draw_rect(0, 0, buildingSize.x, buildingSize.y);
	PM::pop_settings();
	world->DrawAll(cam);
	Engine::GetFont().Draw("When you run, it makes sound", 200+170, WALLTHICKNESS+55,
		1.f, true, PM::AlignType::center);
	Engine::GetFont().Draw("Enemies will hear you. Be careful!", 200+170, WALLTHICKNESS+5,
		1.f, true, PM::AlignType::center);
	Engine::GetFont().Draw("When you rush to the door, it is fast but", ROOMXSIZE * 2 - WALLTHICKNESS, WALLTHICKNESS+55,
		1.f, true, PM::AlignType::center);
	Engine::GetFont().Draw("enemies will hear sound and come to catch you.", ROOMXSIZE * 2 - WALLTHICKNESS, WALLTHICKNESS+5,
		1.f, true, PM::AlignType::center);


	Engine::GetFont().Draw("Blue NPC make a scream and Guards come to catch you", ROOMXSIZE*2, ROOMYSIZE + WALLTHICKNESS+5,
		1.f, true, PM::AlignType::center);
	GetGSComponent<EffectManager>()->DrawAll(cam);

	Engine::GetGSComponent<PM::UI::UIManager>()->Draw();
}

void Tuto::MapLoad()
{
	FloorF1RoomLoad();
	FloorF2RoomLoad();
	world->Add(new PM::GameObject("", { 160,0 },
		{ ROOMXSIZE,ROOMYSIZE }, "assets/GameObject/Room/Tuto/Tuto1.png"));
	world->Add(new PM::GameObject("", { ROOMXSIZE,0 }, { ROOMXSIZE,ROOMYSIZE }, "assets/GameObject/Room/Tuto/Tuto2.png"));
	world->Add(new PM::GameObject("", { ROOMXSIZE*2,0 }, { ROOMXSIZE,ROOMYSIZE }, "assets/GameObject/Room/Tuto/Tuto3.png"));

	FloorF1ObjLoad();
	FloorF2ObjLoad();


	

}


void Tuto::FloorF2RoomLoad()
{
	float floor = ROOMYSIZE;
	Wall* B3_1L = new Wall("F2_1L", { 0,floor }, { WALLTHICKNESS,ROOMYSIZE });
	Wall* B3_4R = new Wall("F2_3R", { ROOMXSIZE * 4 - WALLTHICKNESS,floor }, { WALLTHICKNESS * 2,ROOMYSIZE - 125 - WALLTHICKNESS });
	//Wall* B3_6R = new Wall("F2_7R", { ROOMXSIZE * 7 - WALLTHICKNESS,floor }, { WALLTHICKNESS,ROOMYSIZE });
	
	world->AddRoom(
		new PM::Room("F2_1_4",
			{ 0,floor }, glm::fvec2{ ROOMXSIZE * 4, ROOMYSIZE },
			B3_1L, B3_4R, "assets/Level/NoImage.png"));
	world->Add(B3_1L);
	world->Add(B3_4R);
}

void Tuto::FloorF2ObjLoad()
{
	float floor = ROOMYSIZE;

	world->Add(new Goal({ 300, floor + WALLTHICKNESS }));
	world->Add(new Stair("2FDOWN", { ROOMXSIZE * 4 - ROOMXSIZE / 2,floor + WALLTHICKNESS }, "1FUP", true));

	//key

	world->Add(new Hide_Obj("B3_2Hide", { ROOMXSIZE * 1 + ROOMXSIZE / 2 - 100,floor + WALLTHICKNESS }), 1);
	
	NPC_generator npcGenerator{ playerptr };
	npcGenerator.setting(EntityType::Enemy_NPC, NPCLevelList::Normal, false);
	npcGenerator.settingType(EntityType::Worker_NPC);
	Basic_NPC*  tempEnemy = npcGenerator.generate("B3_worker1", glm::vec2{ ROOMXSIZE * 2 + ROOMXSIZE / 2 - 100,floor + WALLTHICKNESS });
	tempEnemy->AddTaskAI(new Task_AI_pointToPoint{ tempEnemy,{ROOMXSIZE * 0 + 100,floor + WALLTHICKNESS },{ROOMXSIZE*4 - 250,floor + WALLTHICKNESS} });
	world->Add(tempEnemy);



}

void Tuto::FloorF1RoomLoad()
{
	float floor = 0;
	Wall* F1_1L = new Wall("F1_1L", { 0,floor }, { WALLTHICKNESS,ROOMYSIZE });
	SideDoor* F1_3R = new SideDoor("", { ROOMXSIZE * 2 - WALLTHICKNESS,floor + WALLTHICKNESS });

	
	Wall* F1_4R = new Wall("F1_3R", { ROOMXSIZE * 4 - WALLTHICKNESS,floor + WALLTHICKNESS });;
	
	world->AddRoom(
		new PM::Room("B4_12",
			{ 0,floor }, glm::fvec2{ ROOMXSIZE * 2, ROOMYSIZE },
			F1_1L, F1_3R, "assets/Level/NoImage.png"));

	world->AddRoom(
		new PM::Room("B4_34",
			{ ROOMXSIZE * 2,floor }, glm::fvec2{ ROOMXSIZE*2, ROOMYSIZE },
			F1_3R, F1_4R, "assets/Level/NoImage.png"));



	world->Add(F1_1L);
	world->Add(F1_3R);
	world->Add(F1_4R);

}

void Tuto::FloorF1ObjLoad()
{
	float floor = 0;
	//world->Add(new Hide_Obj("B3_2Hide", { ROOMXSIZE * 1 + ROOMXSIZE / 2 - 100,floor + WALLTHICKNESS }), 1);
	world->Add(new Stair("1FUP", { ROOMXSIZE * 4 - ROOMXSIZE / 2,floor + WALLTHICKNESS }, "2FDOWN", false));
}
