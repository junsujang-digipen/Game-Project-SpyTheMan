/*--------------------------------------------------------------
File Name: OpenDoorSequence.cpp
Project: Spy The Man
Author: Junsu Jang, Seongwon Jang
Creation date: 11/9/2021
-----------------------------------------------------------------*/
#include "HideSequence.h"
#include "../Object/Hide_Obj.h"
#include "../Player.h"
#include "../../Engine/Engine.h"
#include "../../Engine/SoundManager.h"

HideSequence::HideSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* door) :Sequence_base(type,player,door) {
	ChangeState(&stateGoing);
}

void HideSequence::Update(double dt)
{
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void HideSequence::State_going::Enter(HideSequence* sq)
{
	static_cast<Player*>(sq->obj1)->GoTo(sq->obj2->GetPosition() + glm::vec2(40, 0));
}

void HideSequence::State_going::Update(HideSequence*, double)
{
	Engine::GetLogger().LogVerbose("going to the door");
}

void HideSequence::State_going::TestForExit(HideSequence* sq)
{

	Player* player = static_cast<Player*>(sq->obj1);
	Hide_Obj* closet = static_cast<Hide_Obj*>(sq->obj2);

	const glm::vec2 pPos = player->GetPosition();
	const glm::vec2 dPos = closet->GetPosition();
	const glm::vec2 pSize = player->GetSize();
	const glm::vec2 dSize = closet->GetSize();
	if (player->Isinterrupted() || closet->Isinterrupted())
	{
		player->SetGoTo(false);
		player->SetInterrupted(false);
		closet->SetInterrupted(false);
		sq->setIsValid(false);
	}
	else if (player->DoesCollideWith(closet))
	{
		sq->ChangeState(&sq->stateHiding);
	}
}
void HideSequence::State_Hiding::Enter(HideSequence* sq)
{
	Engine::GetSoundManager().PlaySound("closet", Engine::GetGSComponent<PM::World>()->GetPlayerPtr()->GetPosition());
	Player* player = static_cast<Player*>(sq->obj1);
	Hide_Obj* closet = static_cast<Hide_Obj*>(sq->obj2);

	const glm::vec2 pPos = player->GetPosition();
	const glm::vec2 cPos = closet->GetPosition();
	const glm::vec2 pSize = player->GetSize();
	const glm::vec2 cSize = closet->GetSize();	
}

void HideSequence::State_Hiding::Update(HideSequence*, double)
{

}

void HideSequence::State_Hiding::TestForExit(HideSequence*sq)
{
	// if obj1 and obj2 animation is ended then set is valid or animation is last then just do setIsvalid
	Player* player = static_cast<Player*>(sq->obj1);

	player->SetHide(true);
	player->SetCloset(true);
	sq->setIsValid(false);
}
