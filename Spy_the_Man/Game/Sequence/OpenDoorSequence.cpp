/*--------------------------------------------------------------
File Name: OpenDoorSequence.cpp
Project: Spy The Man
Author: Junsu Jang, Seongwon Jang
Creation date: 11/9/2021
-----------------------------------------------------------------*/
#include "OpenDoorSequence.h"
#include "../Object/SideDoor.h"
#include "../Player.h"
#include "../../Engine/Engine.h"
#include "../../Engine/SoundManager.h"

OpenDoorSequence::OpenDoorSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* door) :Sequence_base(type,player,door) {
	ChangeState(&stateGoing);
}

void OpenDoorSequence::Update(double dt)
{
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void OpenDoorSequence::State_going::Enter(OpenDoorSequence* sq)
{
	static_cast<Player*>(sq->obj1)->GoTo(sq->obj2->GetPosition());
}

void OpenDoorSequence::State_going::Update(OpenDoorSequence*, double)
{
	Engine::GetLogger().LogVerbose("going to the door");
}

void OpenDoorSequence::State_going::TestForExit(OpenDoorSequence* sq)
{

	Player* player = static_cast<Player*>(sq->obj1);
	SideDoor* door = static_cast<SideDoor*>(sq->obj2);

	const glm::vec2 pPos = player->GetPosition();
	const glm::vec2 dPos = door->GetPosition();
	const glm::vec2 pSize = player->GetSize();
	const glm::vec2 dSize = door->GetSize();
	if (player->Isinterrupted() || door->Isinterrupted())
	{
		player->SetGoTo(false);
		player->SetInterrupted(false);
		door->SetInterrupted(false);
		sq->setIsValid(false);
	}
	else if (pPos.x < dPos.x && pPos.x + pSize.x > dPos.x)
	{
		player->SetGoTo(false);
		sq->ChangeState(&sq->stateOpenDoor);
	}
	else if (pPos.x < dPos.x + dSize.x && pPos.x + pSize.x > dPos.x + dSize.x)
	{
		player->SetGoTo(false);
		sq->ChangeState(&sq->stateOpenDoor);
	}
}
void OpenDoorSequence::State_openDoor::Enter(OpenDoorSequence* )
{
	Engine::GetSoundManager().PlaySound("doorOpen", Engine::GetGSComponent<PM::World>()->GetPlayerPtr()->GetPosition());
	Engine::GetLogger().LogVerbose("door is open");
	//Player* player = static_cast<Player*>(sq->obj1);
	
	//set player animation or state
}

void OpenDoorSequence::State_openDoor::Update(OpenDoorSequence*, double)
{

}

void OpenDoorSequence::State_openDoor::TestForExit(OpenDoorSequence*sq)
{
	// if obj1 and obj2 animation is ended then set is valid or animation is last then just do setIsvalid
	SideDoor* door = static_cast<SideDoor*>(sq->obj2);
	door->OpenAndClose();
	sq->setIsValid(false);
}
