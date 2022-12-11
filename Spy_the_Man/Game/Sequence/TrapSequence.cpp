/*--------------------------------------------------------------
File Name: TrapSequence.cpp
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 4/24/2022
-----------------------------------------------------------------*/
#include "TrapSequence.h"
#include "../Player.h"
#include "../Game/UI/Phone.h"
#include "../Engine/Engine.h"
#include "../Engine/UI/Mouse.h"
#include "../Object/Action/ActionManager.h"
#include "../../Engine/SoundManager.h"

TrapSequence::TrapSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* obj)
	: Sequence_base(type, player, obj)
{
	ChangeState(&stateTrap);
}

void TrapSequence::Update(double dt) {
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void TrapSequence::State_Trap::Enter(TrapSequence*) {
	Engine::GetSoundManager().PlaySound("UIButton", Engine::GetGSComponent<PM::World>()->GetPlayerPtr()->GetPosition());
	Engine::GetGSComponent<Phone>()->setIsValid(false);
	Engine::GetGSComponent<PM::World>()->SetCanUseButton(false);
}

void TrapSequence::State_Trap::Update(TrapSequence* sq, double) {
	Player* player = static_cast<Player*>(sq->obj1);
	PM::World* world = Engine::GetGSComponent<PM::World>();
	if (Engine::GetInput().GetWorldMousePos().x < player->GetCenter().x) {
		if (player->GetCenter().x - world->GetFloorSize(player).x > 625) {
			player->DrawTrap(true, true);
			sq->left = true;
		}
		else {
			player->DrawTrap(true, false);
			sq->left = false;
		}
	}
	else {
		if (world->GetFloorSize(player).y - player->GetCenter().x > 625) {
			player->DrawTrap(true, false);
			sq->left = false;
		}
		else {
			player->DrawTrap(true, true);
			sq->left = true;
		}
	}
	if (Engine::GetInput().LeftMouseIsClicked() == true) {
		Engine::GetSoundManager().PlaySound("SlowTrap", player->GetPosition());
		sq->select = true;
	}
}

void TrapSequence::State_Trap::TestForExit(TrapSequence* sq) {
	if (sq->select == true) {
		Player* player = static_cast<Player*>(sq->obj1);
		player->DrawTrap(false);
		if (sq->left == true) {
			sq->trapptr = new Trap({ player->GetCenter().x - 600, player->GetPosition().y });
		}
		else {
			sq->trapptr = new Trap({ player->GetCenter().x + 100, player->GetPosition().y });
		}
		Engine::GetGSComponent<PM::World>()->Add(sq->trapptr);
		Engine::GetGSComponent<PM::World>()->SetCanUseButton(true);
		Engine::GetGSComponent<Phone>()->setIsValid(true);
		Engine::GetGSComponent<ActionManager>()->GetAction(SkillType::SlowTrap)->SetTimer(0);
		sq->setIsValid(false);
	}
}