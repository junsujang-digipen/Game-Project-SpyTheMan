/*--------------------------------------------------------------
File Name: DroneSequence.cpp
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 3/13/2021
-----------------------------------------------------------------*/
#include <algorithm>
#include "DroneSequence.h"
#include "../Player.h"
#include "../../Engine/UI/Button.h"
#include "../Engine/Engine.h"
#include "../Engine/SoundManager.h"
#include "../Game/UI/Phone.h"
#include "../Engine/UI/Image.h"
#include "../Engine/Helper_func_map.h"
#include "../Engine/Wave.h"
#include "../Object/Action/ActionManager.h"
#include "../Drone.h"
#include "../ValidManager.h"

DroneSequence::DroneSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* obj)
	: Sequence_base(type, player, obj)
{
	ChangeState(&stateBegin);
}
DroneSequence::~DroneSequence()
{
	Engine::GetGSComponent<ValidManager>()->SetButton(true);
}

void DroneSequence::Update(double dt) {
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void DroneSequence::State_Begin::Enter(DroneSequence*)
{
	
	Engine::GetGSComponent<Phone>()->setIsValid(false);
	Engine::GetGSComponent<PM::World>()->SetCanUseButton(false);
}

void DroneSequence::State_Begin::Update(DroneSequence*, double)
{
	
}

void DroneSequence::State_Begin::TestForExit(DroneSequence* sq)
{
	
		sq->ChangeState(&sq->stateBlackScreen);
	
}

void DroneSequence::State_BlackScreen::Enter(DroneSequence*)
{
	blackImage = new PM::UI::Rect("b",glm::vec2{ Engine::GetWindow().GetSize().x,0},Engine::GetWindow().GetSize(),UILayer::SKILLUI,{0,0,0,255});
	blackImage->SetVelocity({ -Engine::GetWindow().GetSize().x * 3 ,0 });
}

void DroneSequence::State_BlackScreen::Update(DroneSequence*, double)
{
}

void DroneSequence::State_BlackScreen::TestForExit(DroneSequence* sq)
{
	if(blackImage->GetPosition().x <=0)
	{
		blackImage->SetPosition({ 0,0 });
		blackImage->Destroy();
		blackImage = nullptr;
		sq->ChangeState(&sq->stateDrone);
	}
}


void DroneSequence::State_Drone::Enter(DroneSequence* sq) {
	Player* player = static_cast<Player*>(sq->obj1);
	sq->droneptr = new Drone(player->GetPosition());
	player->SetDrone(true);
	Engine::GetGSComponent<PM::World>()->Add(sq->droneptr);
	Engine::GetCamera().bindTarget(sq->droneptr);
	
	blackImage = new PM::UI::Rect("b", glm::vec2{0,0 }, Engine::GetWindow().GetSize(), UILayer::SKILLUI,{0,0,0,255});
	if (find(Engine::GetSkillManager().CurrSkills().begin(), Engine::GetSkillManager().CurrSkills().end(), SkillType::DroneSound) 
		!= Engine::GetSkillManager().CurrSkills().end()) {
		makeSoundButton = new PM::UI::TextButton("", { 1100,100 }, { 150,150 }, "Sound", { 0,0,0,150 }, {255,255,255}, UILayer::SKILLUI);
	}
	else {
		makeSoundButton = new PM::UI::TextButton("", { INT_MAX,INT_MAX }, { 150,150 }, "Sound", { 0,0,0 ,150 }, {255,255,255}, UILayer::SKILLUI);
	}
	destroyButton = new PM::UI::TextButton("", { 1300,100 }, { 150,150 }, "Stop", { 0,0,0,150 }, { 255,255,255 }, UILayer::SKILLUI);
}

void DroneSequence::State_Drone::Update(DroneSequence*, double dt) {
	blackImage->SetColor(helper::Lerp(blackImage->GetColor(), glm::vec4{ 55,55,55,100 }, static_cast<float>(dt)));
}

void DroneSequence::State_Drone::TestForExit(DroneSequence* sq) {
	if (sq->droneptr->IsFinish() == true) {
		blackImage->Destroy();
		makeSoundButton->Destroy();
		destroyButton->Destroy();
		sq->ChangeState(&sq->stateEnd);
	}
	if (makeSoundButton->IsReleased() == true) {
		Engine::GetSoundManager().PlaySound("dronesound", sq->droneptr->GetCenter());
		new PM::Wave(sq->droneptr, 6, 2);
		sq->droneptr->SetFinish(true);
	}
	if (destroyButton->IsReleased() == true) {
		sq->droneptr->SetFinish(true);
	}
}

void DroneSequence::State_End::Enter(DroneSequence*) {}

void DroneSequence::State_End::Update(DroneSequence*, double dt) {
	deadtime -= dt;
}

void DroneSequence::State_End::TestForExit(DroneSequence* sq) {
	if (deadtime <= 0) {
		Player* player = static_cast<Player*>(sq->obj1);
		Engine::GetCamera().bindTarget(player);
		player->SetDrone(false);
		Engine::GetGSComponent<Phone>()->setIsValid(true);
		Engine::GetGSComponent<PM::World>()->SetCanUseButton(true);
		Engine::GetGSComponent<ActionManager>()->GetAction(SkillType::Drone)->SetTimer(0);
		sq->setIsValid(false);
	}
}