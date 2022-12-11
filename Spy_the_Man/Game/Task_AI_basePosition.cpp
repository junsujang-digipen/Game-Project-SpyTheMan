/*--------------------------------------------------------------
File Name: Task_AI_basePosition.cpp
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/2/2021
-----------------------------------------------------------------*/
#include "Task_AI_basePosition.h"
#include "../Engine/Helper_func_vec.h"
#include <gsl/gsl>
#include "../Engine/Engine.h"
#include "../Engine/NPC_base.h"
#include "../Engine/World.h"
#include "Basic_NPC.h"



Task_AI_basePosition::Task_AI_basePosition(PM::GameObject* AIOwner, glm::vec2 basePosition, glm::vec3 seeingVec, bool pp,const double tLimt)
	: AI(AIOwner), basePosition(basePosition),
	seeingVec(seeingVec),precisePosition(pp),TimeLimit(tLimt)
{
	ChangeState(&(AI<Task_AI_basePosition>::stateStart));
}

Task_AI_basePosition::Task_AI_basePosition(PM::GameObject* newOwner, Task_AI_basePosition& rhs):AI(newOwner), basePosition(rhs.basePosition),
seeingVec(rhs.seeingVec), precisePosition(rhs.precisePosition), TimeLimit(rhs.TimeLimit)
{
}

void Task_AI_basePosition::start()
{
	ChangeState(&stateGoing);
}

void Task_AI_basePosition::State_going::Enter(Task_AI_basePosition* ai)
{
	Engine::GetLogger().LogDebug("Enter " + GetName());
	//
	//
	
	if (ai->precisePosition == false) {
		PM::NPC_base* owner = dynamic_cast<PM::NPC_base*>(ai->AIOwner);
		PM::World* world = Engine::GetGSComponent<PM::World>();
		PM::Chunk& chunk = world->GetChunk(1, ai->basePosition);
		PM::Room* room = chunk.roomPtr;
		glm::vec2 roomPos = room->GetPosition();
		glm::vec2 roomSize = room->GetSize();
		//ai->basePosition = glm::vec2{ Tpoint };
		ai->basePosition =
			glm::vec2{ Engine::GetRandomNum(
				(roomPos.x + owner->GetSize().x),
				(roomPos.x + roomSize.x - owner->GetSize().x*1.5)) ,
			ai->basePosition.y };
	}
}

void Task_AI_basePosition::State_going::Update(Task_AI_basePosition* ai, double /*dt*/)
{
	Basic_NPC* owner = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	owner->go(ai->basePosition, owner->getSpeed());
}

void Task_AI_basePosition::State_going::TestForExit(Task_AI_basePosition* ai)
{
	PM::GameObject* owner = ai->AIOwner;
	glm::vec2 base = ai->basePosition;
	glm::vec2 direction{ base - owner->GetPosition() };
	PM::World* world = Engine::GetGSComponent<PM::World>();
	const int baseRoomNum{ world->GetFloorNumber(base) };
	const int ownerRoomNum{ world->GetFloorNumber(owner->GetPosition()) };
	if (baseRoomNum == ownerRoomNum && direction.x* direction.x <= owner->GetSize().x* owner->GetSize().x) {
		ai->ChangeState(&ai->stateWorking);
	}
}

void Task_AI_basePosition::State_working::Enter(Task_AI_basePosition* ai)
{
	Engine::GetLogger().LogDebug("Enter " + GetName());
	
	Basic_NPC* owner = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	const glm::vec3 sv = ai->seeingVec;
	if (sv != glm::vec3{}) {
		owner->setSight(sv);
	}
	const_cast<glm::vec2&>(owner->GetVelocity()) = glm::vec2{};
}

void Task_AI_basePosition::State_working::Update(Task_AI_basePosition* /*ai*/, double dt)
{
	Engine::GetLogger().LogVerbose("Working!!");
	nowT += dt;
}

void Task_AI_basePosition::State_working::TestForExit(Task_AI_basePosition* ai)
{
	if (nowT > ai->TimeLimit) {
		Engine::GetLogger().LogDebug("End " + GetName());

		ai->setIsValid(false);
		nowT = 0;
	}
	PM::GameObject* owner = ai->AIOwner;
	glm::vec2 base = ai->basePosition;
	glm::vec2 direction{ base - owner->GetPosition() };
	PM::World* world = Engine::GetGSComponent<PM::World>();
	const int baseRoomNum{ world->GetFloorNumber(base) };
	const int ownerRoomNum{ world->GetFloorNumber(owner->GetPosition()) };
	if (baseRoomNum != ownerRoomNum || direction.x* direction.x > owner->GetSize().x* owner->GetSize().x) {
		ai->ChangeState(&ai->stateGoing);
	}
}

