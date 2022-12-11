/*--------------------------------------------------------------
File Name: Master_AI.cpp
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/2/2021
-----------------------------------------------------------------*/
#include <gsl/gsl>
#include <algorithm>
#include "Master_AI.h"
#include "../Engine/Engine.h"
#include "Basic_NPC.h"
#include "../Engine/Collision.h"
#include "WorldGuardLevel.h"
#include "Task_AI_basePosition.h"
#include "Task_AI_pointToPoint.h"

void Master_AI::giveTask(Basic_NPC* npc, TaskAISaveData &tdata)
{

	switch (tdata.taskType)
	{
	case TaskAIList::PointToPoint:
		npc->AddTaskAI(new Task_AI_pointToPoint{ npc ,*dynamic_cast<Task_AI_pointToPoint*>(tdata.TaskAI)});
		break;
	case TaskAIList::BasePosition:
		npc->AddTaskAI(new Task_AI_basePosition{npc,*dynamic_cast<Task_AI_basePosition*>(tdata.TaskAI)});
		break;
	default:
		break;
	}
}

Master_AI::Master_AI():Component()
{

}

Master_AI::~Master_AI()
{
	for (auto& tl:taskList) {
		delete tl.TaskAI;
		tl.TaskAI = nullptr;
	}
	taskList.clear();
	NPCPool.clear();
}

void Master_AI::Update(double /*dt*/)
{
	//Will be implemented
	isBlueEnemyDetact = false;
	isRedEnemyDetact = false;
	for (auto& npc:NPCPool) {
		if (npc->isNeedNewTask() == true && taskList.empty() == false) {
			
			if (npc->GetEntityType() == taskList[NumberOfTask].type) {
				giveTask(npc, taskList[NumberOfTask]);
			}
			NumberOfTask = (NumberOfTask + 1) % taskList.size();
		}
		
		if (npc->getGaze() >= 100.) {
			if (npc->GetEntityType() == EntityType::Worker_NPC) {
				isBlueEnemyDetact = true;
			}
			else if (npc->GetEntityType() == EntityType::Enemy_NPC) {
				isRedEnemyDetact = true;
			}
		}
	}
	for (auto& npc : NotControledNPCPool) {
		if (npc->getGaze() >= 100.) {
			if (npc->GetEntityType() == EntityType::Worker_NPC) {
				isBlueEnemyDetact = true;
			}
			else if (npc->GetEntityType() == EntityType::Enemy_NPC) {
				isRedEnemyDetact = true;
			}
		}
	}

};
void Master_AI::EnrollNPC(Basic_NPC* obj, bool isControlled)
{
	if (isControlled == false) {
		NotControledNPCPool.push_back(obj);
	}
	else {
		NPCPool.push_back(obj);
	}
}
void Master_AI::removeNPC(Basic_NPC* npc)
{
	npc->removeMe();

	for (auto& e : NPCPool) {
		if (e->sameRoomCheck(npc)) {
			e->setGaze(100.);
		}
	}
	for (auto& e : NotControledNPCPool) {
		if (e->sameRoomCheck(npc)) {
			e->setGaze(100.);
		}
	}

	std::vector<Basic_NPC*>::iterator npcItr{std::find(NPCPool.begin(), NPCPool.end(), npc)};
	if (npcItr != NPCPool.end()) {
		NPCPool.erase(npcItr);
	}
	npcItr={ std::find(NotControledNPCPool.begin(), NotControledNPCPool.end(), npc) };
	if (npcItr != NotControledNPCPool.end()) {
		NotControledNPCPool.erase(npcItr);
	}
	Engine::GetGSComponent<WorldGuardLevel>()->Level += 1.;
}

void Master_AI::EnrollTask(PM::Component* data, TaskAIList tt, EntityType t, NPCLevelList l)
{
	taskList.emplace_back();
	taskList[taskList.size() - 1].level = l;
	taskList[taskList.size() - 1].taskType = tt;
	taskList[taskList.size() - 1].type = t;
	taskList[taskList.size() - 1].TaskAI = data;
}

TaskAISaveData::TaskAISaveData(PM::Component* task, TaskAIList tt, EntityType t, NPCLevelList l):TaskAI(task),taskType(tt),type(t),level(l)
{
}

//TaskAISaveData::~TaskAISaveData()
//{
//	delete TaskAI;
//}
