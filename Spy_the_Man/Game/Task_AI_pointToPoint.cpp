/*--------------------------------------------------------------
File Name: Task_AI_basePosition.cpp
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/2/2021
-----------------------------------------------------------------*/
#include "Task_AI_pointToPoint.h"
#include "../Engine/Helper_func_vec.h"
#include <gsl/gsl>
#include "../Engine/Engine.h"
#include "Basic_NPC.h"
#include "../Engine/World.h"
#include "../Game/Object/Room.h"

Task_AI_pointToPoint::Task_AI_pointToPoint(PM::GameObject* AIOwner, glm::vec2 point1 , glm::vec2 point2, bool pp)
	:AI(AIOwner),firstP(point1),secondP(point2),precisePosition(pp) 
{
	if (AIOwner != nullptr) {
		ChangeState(&(AI<Task_AI_pointToPoint>::stateStart));
		const glm::vec2 pos = AIOwner->GetPosition();
		if (firstP == glm::vec2{}) {
			firstP = pos;
		}
		if (secondP == glm::vec2{}) {
			secondP = pos;
		}
	}
}

Task_AI_pointToPoint::Task_AI_pointToPoint(PM::GameObject* newOwner, Task_AI_pointToPoint& rhs) :AI(newOwner), firstP(rhs.firstP), secondP(rhs.secondP), precisePosition(rhs.precisePosition)
{
	//AIOwner = newOwner;

	ChangeState(&(AI<Task_AI_pointToPoint>::stateStart));
	const glm::vec2 pos = AIOwner->GetPosition();
	if (firstP == glm::vec2{}) {
		firstP = pos;
	}
	if (secondP == glm::vec2{}) {
		secondP = pos;
	}
}

void Task_AI_pointToPoint::start()
{
	ChangeState(&stateGoing);
}

void Task_AI_pointToPoint::State_going::Enter(Task_AI_pointToPoint* ai)
{
	switchPoint();
	PM::NPC_base* owner = dynamic_cast<PM::NPC_base*>(ai->AIOwner);

	//Engine::GetLogger().LogDebug("Enter " + GetName());

	glm::vec2 Tpoint{ whatPoint == true ? ai->firstP:ai->secondP};

	PM::World* world = Engine::GetGSComponent<PM::World>();
	PM::Chunk& chunk = world->GetChunk(1,Tpoint);
	PM::Room* room = chunk.roomPtr;
	glm::vec2 roomPos = room->GetPosition();
	glm::vec2 roomSize = room->GetSize();
	ai->targetPosition = glm::vec2{ Tpoint};
	if (ai->precisePosition == false) {
		ai->targetPosition = glm::vec2{ Engine::GetRandomNum((roomPos.x + owner->GetSize().x / 2),(roomPos.x + roomSize.x - owner->GetSize().x - 100)) ,Tpoint.y };
	}

}

void Task_AI_pointToPoint::State_going::Update(Task_AI_pointToPoint* ai, double /*dt*/)
{
	Basic_NPC* owner = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	owner->go(ai->targetPosition, owner->getSpeed());
}

void Task_AI_pointToPoint::State_going::TestForExit(Task_AI_pointToPoint* ai)
{
	PM::GameObject* owner = ai->AIOwner;
	glm::vec2 base = ai->targetPosition;
	glm::vec2 direction{ base - owner->GetPosition() };
	PM::World* world = Engine::GetGSComponent<PM::World>();
	const int baseRoomNum{ world->GetFloorNumber(base) };
	const int ownerRoomNum{ world->GetFloorNumber(owner->GetPosition()) };
	if (baseRoomNum == ownerRoomNum && direction.x* direction.x <= owner->GetSize().x* owner->GetSize().x) {
		ai->ChangeState(&ai->stateWorking);
	}
}

void Task_AI_pointToPoint::State_working::Enter(Task_AI_pointToPoint* ai)
{
	Engine::GetLogger().LogDebug("Enter " + GetName());

	PM::GameObject* owner = ai->AIOwner;
	const_cast<glm::vec2&>(owner->GetVelocity()) = glm::vec2{};
}

void Task_AI_pointToPoint::State_working::Update(Task_AI_pointToPoint* ai, double dt)
{
	//working animation?
	Engine::GetLogger().LogVerbose("Working!!");
	nowT += dt;
	ai->newTaskTime += dt;
}

void Task_AI_pointToPoint::State_working::TestForExit(Task_AI_pointToPoint* ai)
{
	if (ai->newTaskTimeLimit < ai->newTaskTime) {
		ai->setIsValid(false);
		ai->newTaskTime = 0;
		return;
	}
	
	PM::GameObject* owner = ai->AIOwner;
	glm::vec2 base = ai->targetPosition;
	glm::vec2 direction{ base - owner->GetPosition() };
	PM::World* world = Engine::GetGSComponent<PM::World>();
	const int baseRoomNum{ world->GetFloorNumber(base) };
	const int ownerRoomNum{ world->GetFloorNumber(owner->GetPosition()) };
	if (baseRoomNum != ownerRoomNum || helper::length(direction) > owner->GetSize().x ) {
		ai->ChangeState(&ai->stateGoing);
	}
	if (nowT > TimeLimit) {
		Engine::GetLogger().LogDebug("End " + GetName());

		ai->ChangeState(&ai->stateGoing);
		nowT = 0;
	}
}