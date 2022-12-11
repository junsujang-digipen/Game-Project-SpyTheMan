/*--------------------------------------------------------------
File Name: NPC_OpenDoorSequence.cpp
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/10/2021
-----------------------------------------------------------------*/
#include "NPC_OpenDoorSequence.h"
#include "../Object/SideDoor.h"
#include "../Player.h"
#include "../../Engine/Engine.h"

NPC_OpenDoorSequence::NPC_OpenDoorSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* door) :Sequence_base(type, player, door) {
	ChangeState(&stateOpenDoor);
	Engine::GetLogger().LogVerbose("OpenDoor sequence St!!!!!!!!!!11111");
}

void NPC_OpenDoorSequence::Update(double dt)
{
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void NPC_OpenDoorSequence::State_openDoor::Enter(NPC_OpenDoorSequence* sq)
{
	SideDoor* door = static_cast<SideDoor*>(sq->obj2);
	if (door->GetIsOpen() == false) {
		door->OpenAndClose();
	}
}

void NPC_OpenDoorSequence::State_openDoor::Update(NPC_OpenDoorSequence* /*sq*/, double)
{
	
}

void NPC_OpenDoorSequence::State_openDoor::TestForExit(NPC_OpenDoorSequence* sq)
{
	
	//Player* enemy = static_cast<Player*>(sq->obj1);
	SideDoor* door = static_cast<SideDoor*>(sq->obj2);
	//set player animation or state
	
	// if obj1 and obj2 animation is ended then set is valid or animation is last then just do setIsvalid
	//if (abs(sq->obj1->GetPosition().x - door->GetPosition().x) >= sq->obj1->GetSize().x * 1.5f){
	//	Engine::GetLogger().LogVerbose("door is open");
	//	sq->setIsValid(false);
	//	new NPC_CloseDoorSequence{PM::SequenceType::Other,sq->obj1,sq->obj2};
	//}

	if (door->GetIsOpen() == true) {
		sq->setIsValid(false);
		new NPC_CloseDoorSequence{ PM::SequenceType::Other,sq->obj1,sq->obj2 };
	}
	if (door->GetIsOpen() == false) {
		sq->setIsValid(false);
	}
	
}

void NPC_CloseDoorSequence::State_closeDoor::Enter(NPC_CloseDoorSequence*)
{
}

void NPC_CloseDoorSequence::State_closeDoor::Update(NPC_CloseDoorSequence* /*sq*/, double)
{
}

void NPC_CloseDoorSequence::State_closeDoor::TestForExit(NPC_CloseDoorSequence* sq)
{
	SideDoor* door = static_cast<SideDoor*>(sq->obj2);
	//set player animation or state

	// if obj1 and obj2 animation is ended then set is valid or animation is last then just do setIsvalid
	if(abs( sq->obj1->GetPosition().x + sq->obj1->GetSize().x/2.f- door->GetPosition().x + door->GetSize().x/2.f) >= sq->obj1->GetSize().x *1.5f)
	{
		if (door->GetIsOpen() == true) {
			door->OpenAndClose();
		}
		Engine::GetLogger().LogVerbose("door is closed");
		sq->setIsValid(false);
	}
	if (door->GetIsOpen() == false) {
		sq->setIsValid(false);
	}
}

NPC_CloseDoorSequence::NPC_CloseDoorSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* door) :Sequence_base(type, player, door)
{
	Engine::GetLogger().LogVerbose("CloseDoor sequence St!!!!!!!!!!11111");
	ChangeState(&stateCloseDoor);
}

void NPC_CloseDoorSequence::Update(double dt)
{
	currState->Update(this, dt);
	currState->TestForExit(this);
}
