/*--------------------------------------------------------------
File Name: Master_AI.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/2/2021
-----------------------------------------------------------------*/
#pragma once
#include "../Engine/Component.h"
#include <vector>
#include <initializer_list>
#include <list>
#include "../Engine/Declaration.h"


namespace PM {
	class GameObject;
}
class Basic_NPC;

struct TaskAISaveData{
	PM::Component* TaskAI{};
	EntityType type{};
	NPCLevelList level{};
	TaskAIList taskType{};
	TaskAISaveData() = default;
	TaskAISaveData(PM::Component*, TaskAIList tt, EntityType t, NPCLevelList l = NPCLevelList::Normal);
	//~TaskAISaveData();
};

class Master_AI : public PM::Component{//FSM 베이스가 필요한가?
	// 
	std::vector<TaskAISaveData> taskList{};
	std::vector<Basic_NPC*> NPCPool{};
	std::vector<Basic_NPC*> NotControledNPCPool{};
	unsigned NumberOfTask{};

	bool isBlueEnemyDetact{false};
	bool isRedEnemyDetact{ false };

	void giveTask(Basic_NPC*, TaskAISaveData&);

public:
	
	Master_AI();
	~Master_AI()override;//todo
	void Update(double) override;
	void EnrollNPC(Basic_NPC*,bool isControlled = true);
	void removeNPC(Basic_NPC*);
	void EnrollTask(PM::Component* data, TaskAIList tt, EntityType t, NPCLevelList l);

	bool getIsBlueEnemyDetact() { return isBlueEnemyDetact; }
	bool getIsRedEnemyDetact() { return isRedEnemyDetact; }
	bool getIsEnemyDetact() { return isRedEnemyDetact || isBlueEnemyDetact; }

};
