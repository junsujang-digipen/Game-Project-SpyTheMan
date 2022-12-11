/*--------------------------------------------------------------
File Name: MoveStairSequence.cpp
Project: Spy The Man
Author: Seongwon Jang
Creation date: 11/10/2021
-----------------------------------------------------------------*/
#include "MoveStairSequence.h"

#include "../Object/Stair.h"
#include "../Player.h"
#include "../../Engine/Engine.h"
#include "../../Engine/SoundManager.h"

MoveStairSequence::MoveStairSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* door) :Sequence_base(type, player, door) {
	ChangeState(&stateGoing);
}

void MoveStairSequence::Update(double dt)
{
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void MoveStairSequence::State_going::Enter(MoveStairSequence* sq)
{
	static_cast<Player*>(sq->obj1)->GoTo(sq->obj2->GetPosition());
}

void MoveStairSequence::State_going::Update(MoveStairSequence*, double)
{
	Engine::GetLogger().LogVerbose("going to the stair");
}

void MoveStairSequence::State_going::TestForExit(MoveStairSequence* sq)
{

	Player* player = static_cast<Player*>(sq->obj1);
	Stair* stair = static_cast<Stair*>(sq->obj2);

	const glm::vec2 pPos = player->GetPosition();
	const glm::vec2 sPos = stair->GetPosition();
	const glm::vec2 pSize = player->GetSize();
	const glm::vec2 sSize = stair->GetSize();
	if (player->Isinterrupted() || stair->Isinterrupted())
	{
		player->SetGoTo(false);
		player->SetInterrupted(false);
		stair->SetInterrupted(false);
		sq->setIsValid(false);
	}
	if (pPos.x + pSize.x > sPos.x + sSize.x / 2 && pPos.x < sPos.x + sSize.x / 2)
	{
		player->SetGoTo(false);
		sq->ChangeState(&sq->stateMoveStair);
	}
	else if (pPos.x < sPos.x + sSize.x / 2 && pPos.x + pSize.x > sPos.x + sSize.x / 2)
	{
		player->SetGoTo(false);
		sq->ChangeState(&sq->stateMoveStair);
	}
}

void MoveStairSequence::State_MoveStair::Enter(MoveStairSequence* /*sq*/)
{
	//set player animation
	//Player* player = static_cast<Player*>(sq->obj1);
	//Stair* stair = static_cast<Stair*>(static_cast<Stair*>(sq->obj2)->GetTarget());

	//const glm::vec2 sPos = stair->GetPosition();
	//const glm::vec2 sSize = stair->GetSize();
	//const glm::vec2 pSize = player->GetSize();
	//player->SetPosition({ sPos.x + sSize.x / 2 - pSize.x / 2 ,sPos.y });
	Engine::GetSoundManager().PlaySound("stair", Engine::GetGSComponent<PM::World>()->GetPlayerPtr()->GetPosition());
	time = 0;
}

void MoveStairSequence::State_MoveStair::Update(MoveStairSequence* sq, double dt)
{
	Player* player = static_cast<Player*>(sq->obj1);
	Stair* stair = static_cast<Stair*>(static_cast<Stair*>(sq->obj2)->GetTarget());
	const glm::vec2 sPos = stair->GetPosition();
	const glm::vec2 sSize = stair->GetSize();
	const glm::vec2 pSize = player->GetSize();
	time += dt;
	//if (time < 1.) {
		//Engine::GetCamera().updateCameraHeight(10.f);
	//}
	//else if(time <2.){
		player->SetPosition({ sPos.x + sSize.x / 2 - pSize.x / 2 ,sPos.y });
		//Engine::GetCamera().updateCameraHeight(-10.f);
	//}


}

void MoveStairSequence::State_MoveStair::TestForExit(MoveStairSequence* sq)
{

	//if (time > 2.) {
		sq->setIsValid(false);
	//}

}
