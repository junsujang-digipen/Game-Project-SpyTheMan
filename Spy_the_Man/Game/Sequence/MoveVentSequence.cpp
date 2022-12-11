/*--------------------------------------------------------------
File Name: MoveStairSequence.cpp
Project: Spy The Man
Author: Seongwon Jang
Creation date: 11/10/2021
-----------------------------------------------------------------*/
#include "MoveVentSequence.h"

#include "../Object/Vent.h"
#include "../Player.h"
#include "../../Engine/Engine.h"
#include "../../Engine/SoundManager.h"

MoveVentSequence::MoveVentSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* door) :Sequence_base(type, player, door) {
	ChangeState(&stateGoing);
}

void MoveVentSequence::Update(double dt)
{
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void MoveVentSequence::State_going::Enter(MoveVentSequence* sq)
{
	static_cast<Player*>(sq->obj1)->GoTo(sq->obj2->GetPosition());
}

void MoveVentSequence::State_going::Update(MoveVentSequence*, double)
{
	Engine::GetLogger().LogVerbose("going to the Vent");
}

void MoveVentSequence::State_going::TestForExit(MoveVentSequence* sq)
{
	Player* player = static_cast<Player*>(sq->obj1);
	VentDoor* stair = static_cast<VentDoor*>(sq->obj2);
	PM::GameObject* stair2 =stair->GetTarget();
	if(stair->GetPosition().y > stair2->GetPosition().y)
	{
		if(player->IsVent() == false)
		{
			player->SetInterrupted(true);
		}
	}
	else if(stair->GetPosition().y < stair2->GetPosition().y)
	{
		if (player->IsVent() == true)
		{
			player->SetInterrupted(true);
		}
	}
	if (player->Isinterrupted() || stair->Isinterrupted())
	{
		player->SetGoTo(false);
		player->SetInterrupted(false);
		stair->SetInterrupted(false);
		sq->setIsValid(false);
	}
	else if (player->GetGoToPos() == false)
	{
		player->SetGoTo(false);
		sq->ChangeState(&sq->stateMoveVent);
	}
}

void MoveVentSequence::State_MoveVent::Enter(MoveVentSequence* sq)
{
	Engine::GetSoundManager().PlaySound("vent", Engine::GetGSComponent<PM::World>()->GetPlayerPtr()->GetPosition());
	//set player animation
	Player* player = static_cast<Player*>(sq->obj1);
	PM::GameObject* stair = static_cast<VentDoor*>(sq->obj2)->GetTarget();
	if (player->IsVent() == true)
	{
		player->SetVent(false);
		player->SetHide(false);
		const glm::vec2 sPos = stair->GetPosition();
		const glm::vec2 sSize = stair->GetSize();
		const glm::vec2 pSize = player->GetSize();
		player->SetPosition({ sPos.x + sSize.x / 2 - pSize.x / 2 ,(ROOMYSIZE * (static_cast<int>(sPos.y) / ROOMYSIZE)) + WALLTHICKNESS});
	}
	else
	{
		player->SetVent(true);
		player->SetHide(true);
		const glm::vec2 sPos = stair->GetPosition();
		const glm::vec2 sSize = stair->GetSize();
		const glm::vec2 pSize = player->GetSize();
		player->SetPosition({ sPos.x + sSize.x / 2 - pSize.x / 2 ,sPos.y });
	}

}

void MoveVentSequence::State_MoveVent::Update(MoveVentSequence*, double)
{

}

void MoveVentSequence::State_MoveVent::TestForExit(MoveVentSequence* sq)
{
	sq->setIsValid(false);
}
