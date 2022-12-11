/*--------------------------------------------------------------
File Name: PushSequence.cpp
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 4/26/2022
-----------------------------------------------------------------*/
#include "PushSequence.h"
#include "../Player.h"
#include "../Game/UI/Phone.h"
#include "../Engine/Engine.h"
#include "../Engine/UI/Mouse.h"
#include "../../Engine/SoundManager.h"

PushSequence::PushSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* obj)
	: Sequence_base(type, player, obj)
{
	ChangeState(&statePush);
}

void PushSequence::Update(double dt) {
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void PushSequence::State_Push::Enter(PushSequence*) {
	Engine::GetSoundManager().PlaySound("UIButton", Engine::GetGSComponent<PM::World>()->GetPlayerPtr()->GetPosition());
	Engine::GetGSComponent<Phone>()->setIsValid(false);
	Engine::GetGSComponent<PM::World>()->SetCanUseButton(false);
}

void PushSequence::State_Push::Update(PushSequence* sq, double) {
	Player* player = static_cast<Player*>(sq->obj1);
	if (Engine::GetInput().GetWorldMousePos().x < player->GetCenter().x) {
		player->DrawPush(true, true);
		sq->left = true;
	}
	else {
		player->DrawPush(true, false);
		sq->left = false;
	}
	if (Engine::GetInput().LeftMouseIsClicked() == true) {
		Engine::GetSoundManager().PlaySound("Push", Engine::GetGSComponent<PM::World>()->GetPlayerPtr()->GetPosition());
		sq->select = true;
	}
}

void PushSequence::State_Push::TestForExit(PushSequence* sq) {
	if (sq->select == true) {
		Player* player = static_cast<Player*>(sq->obj1);
		PM::World* world = Engine::GetGSComponent<PM::World>();
		player->DrawPush(false);
		if (sq->left == true) {
			sq->pushptr = new Push({ world->GetFloorSize(player).x + 25, player->GetPosition().y }, player->GetCenter().x - world->GetFloorSize(player).x - 100, true);
		}
		else {
			sq->pushptr = new Push({ player->GetCenter().x + 100, player->GetPosition().y }, world->GetFloorSize(player).y - player->GetCenter().x - 125, false);
		}
		Engine::GetGSComponent<PM::World>()->Add(sq->pushptr);
		Engine::GetGSComponent<PM::World>()->SetCanUseButton(true);
		Engine::GetGSComponent<Phone>()->setIsValid(true);
		sq->setIsValid(false);
	}
}