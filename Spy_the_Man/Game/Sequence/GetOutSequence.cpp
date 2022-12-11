/*--------------------------------------------------------------
File Name: GetOutSequence.cpp
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/27/2021
-----------------------------------------------------------------*/
#include "GetOutSequence.h"
#include "../Player.h"

GetOutSequence::GetOutSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* obj)
	:Sequence_base(type, player, obj)
{
	ChangeState(&stateGoing);
}

void GetOutSequence::Update(double dt)
{
	currState->Update(this, dt);
	currState->TestForExit(this);
}


void GetOutSequence::State_going::Enter(GetOutSequence* sq)
{
	static_cast<Player*>(sq->obj1)->GoTo({ sq->obj2->GetPosition().x + sq->obj2->GetSize().x / 2 , sq->obj2->GetPosition().y });
}

void GetOutSequence::State_going::Update(GetOutSequence*, double)
{

}

void GetOutSequence::State_going::TestForExit(GetOutSequence* sq)
{
	Player* player = static_cast<Player*>(sq->obj1);
	PM::GameObject* obj = sq->obj2;

	
	if (player->Isinterrupted() || obj->Isinterrupted())
	{
		player->SetGoTo(false);
		player->SetInterrupted(false);
		obj->SetInterrupted(false);
		sq->setIsValid(false);
	}
	else if (player->GetGoToPos() == false)
	{
		player->SetLayer(player->GetLayer() + 1);
		sq->setIsValid(false);
	}
}

