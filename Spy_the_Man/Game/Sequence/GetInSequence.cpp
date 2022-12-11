/*--------------------------------------------------------------
File Name: GetInSequence.cpp
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/27/2021
-----------------------------------------------------------------*/
#include "GetInSequence.h"

#include "../Player.h"

GetInSequence::GetInSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* obj)
	:Sequence_base(type, player, obj)
{
	ChangeState(&stateGoing);
}

void GetInSequence::Update(double dt)
{
	currState->Update(this, dt);
	currState->TestForExit(this);
}


void GetInSequence::State_going::Enter(GetInSequence* sq)
{
	static_cast<Player*>(sq->obj1)->GoTo({ sq->obj2->GetPosition().x + sq->obj2->GetSize().x/2 , sq->obj2->GetPosition().y} );
}

void GetInSequence::State_going::Update(GetInSequence*, double)
{
	
}

void GetInSequence::State_going::TestForExit(GetInSequence* sq)
{
	Player* player = static_cast<Player*>(sq->obj1);
	PM::GameObject* obj =sq->obj2;

	if (player->Isinterrupted() || obj->Isinterrupted())
	{
		player->SetGoTo(false);
		player->SetInterrupted(false);
		obj->SetInterrupted(false);
		sq->setIsValid(false);
	}
	else if (player->GetGoToPos() == false && sq->getIsValid() == true)
	{
		player->SetLayer(player->GetLayer() - 1);
		sq->setIsValid(false);
	}
}
