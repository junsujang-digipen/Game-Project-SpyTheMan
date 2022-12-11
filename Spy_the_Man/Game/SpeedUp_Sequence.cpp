/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: SpeedUp_Sequence.cpp
Project: GAM200
Author: Dong-A Choi
-----------------------------------------------------------------*/
#include "SpeedUp_Sequence.h"

#include "Player.h"
#include "../Engine/Engine.h"
#include "Master_AI.h"
#include "../Engine/Input.h"

namespace PM
{
	class UIManager;
}

SpeedUp_Sequence::SpeedUp_Sequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* nothing) :
	Sequence_base(type, player, nothing),toggled(false),toggleKey(PM::InputKey::Keyboard::M)
{
	currState = &state_toggle_on;
	currState->Enter(this);
}
void SpeedUp_Sequence::Update(double dt)
{
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void SpeedUp_Sequence::State_ToggleOn::Enter(SpeedUp_Sequence* speed_up)
{
	//static_cast<Player*>(speed_up->obj1)->SetXVel(500);
	speed_up->toggled = true;
}
void SpeedUp_Sequence::State_ToggleOn::Update(SpeedUp_Sequence* speed_up, double)
{
	Player* player = static_cast<Player*>(speed_up->obj1);
	if (player->GetCheckRun() == false) {
		player->SetXVel(500);
	}else
	{
		player->SetXVel(350);
	}
}
void SpeedUp_Sequence::State_ToggleOn::TestForExit(SpeedUp_Sequence* speed_up)
{
	if(speed_up->toggleKey.IsKeyReleased())
	{
		speed_up->ChangeState(&speed_up->state_toggle_off);
	}
}

void SpeedUp_Sequence::State_ToggleOff::Enter(SpeedUp_Sequence* speed_up)
{
	speed_up->toggled = false;
	
}
void SpeedUp_Sequence::State_ToggleOff::Update(SpeedUp_Sequence* speed_up, double)
{
	Player* player = static_cast<Player*>(speed_up->obj1);
	if (player->GetCheckRun() == false) {
		player->SetXVel(300);
	}
	else
	{
		player->SetXVel(300);
	}
}
void SpeedUp_Sequence::State_ToggleOff::TestForExit(SpeedUp_Sequence* speed_up)
{
	if (speed_up->toggleKey.IsKeyReleased())
	{
		speed_up->ChangeState(&speed_up->state_toggle_on);
	}
}

