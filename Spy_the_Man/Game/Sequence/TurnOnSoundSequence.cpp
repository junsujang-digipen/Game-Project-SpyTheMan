/*--------------------------------------------------------------
File Name: TurnOnSoundSequence.cpp
Project: Spy The Man
Author: Dong-A Choi
Creation date: 11/27/2021
-----------------------------------------------------------------*/
#include "TurnOnSoundSequence.h"


#include "../Player.h"
#include "../../Engine/Engine.h"
#include "../../Engine/SoundManager.h"


TurnOnSoundSequence::TurnOnSoundSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* radio, std::string soundName, int)
	:Sequence_base(type, player, radio) {
	ChangeState(&stateGoing);
}



void TurnOnSoundSequence::State_going::Update(TurnOnSoundSequence*, double)
{
	
}


void TurnOnSoundSequence::State_going::Enter(TurnOnSoundSequence* sq)
{
	static_cast<Player*>(sq->obj1)->GoTo({sq->obj2->GetPosition().x,sq->obj1->GetPosition().y});
}

void TurnOnSoundSequence::Update(double dt)
{
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void TurnOnSoundSequence::State_going::TestForExit(TurnOnSoundSequence* sq)
{
	Player* player = static_cast<Player*>(sq->obj1);
	if (player->Isinterrupted() || sq->obj2->Isinterrupted())
	{
		player->SetGoTo(false);
		player->SetInterrupted(false);
		sq->obj2->SetInterrupted(false);
		sq->setIsValid(false);
	}
	

	if (player->GetGoToPos() == false)
	{
		Engine::GetSoundManager().PlaySound("radio", sq->obj2->GetPosition());
		sq->setIsValid(false);
	}
}

