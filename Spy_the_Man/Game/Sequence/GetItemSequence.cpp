#include "GetItemSequence.h"

#include "../Game/Player.h"
#include "../Object/Item.h"

class QuestItem;

GetItemSequence::GetItemSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* obj)
	: Sequence_base(type, player, obj)
{
	ChangeState(&stateGoing);
}
void GetItemSequence::Update(double dt)
{
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void GetItemSequence::State_going::Enter(GetItemSequence* sq)
{
	static_cast<Player*>(sq->obj1)->GoTo({ sq->obj2->GetPosition().x + sq->obj2->GetSize().x / 2 , sq->obj1->GetPosition().y });
}

void GetItemSequence::State_going::Update(GetItemSequence*, double)
{

}


void GetItemSequence::State_going::TestForExit(GetItemSequence* sq)
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
		(obj->GetGOComponent<Item>())->GetItem();
		sq->setIsValid(false);
		obj->Destroy();
	}
}

