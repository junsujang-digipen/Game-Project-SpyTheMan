#pragma once
/*--------------------------------------------------------------
File Name: Basic_NPC.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 4/3/2021
-----------------------------------------------------------------*/
#include "../Engine/NPC_base.h"
#include <initializer_list>

class Basic_NPC :public PM::NPC_base {
	//glm::vec2 size{ 250, 250 };
	bool isControlled{ false };
	glm::vec3 checkPos{};

	unsigned NumberofTask{0};
	bool hasImportantTask{false};
	PM::Component* ImportantTask{};
	std::list<PM::Component*> TaskAIComponents{};
	std::list<PM::GameObject*> dropItems{};
	double suspicionGaze{};
	glm::vec3 sight{ -1,0,0 };
	static constexpr double stunTimeMax{4};
	double stunTime{};
	std::list<PM::Entity*> collidingObj{};//not owner
	unsigned int talkResult{ 0 };
	double indiIncreaseGuardLevelSpeed{ 1. };//NPC's increaseGuardLevelSpeed.
	std::vector<PM::Texture*> headMarkImage{};
	unsigned int MarkState{ 0 };
	static constexpr double SoundDetactMax{ 2. };
	double SoundDetact{0.};
	double WindMarkTime{ 0. };
	void BasicAnimation();
	void loadBasicMarkTexture();

	EntityType type{ EntityType::Basic_NPC };
	NPCLevelList NPClevel{ NPCLevelList::Normal };

	std::map<int, std::vector<int>> routesTempo{};

public:
	
	Basic_NPC(PM::GameObject* p,std::string name, 
		glm::vec2 position, 
		EntityType t,
		NPCLevelList l= NPCLevelList::Normal,
		bool isControlled = true, 
		std::string spritePath = "assets/player.json");
	~Basic_NPC();
	void Update(double dt)override;
	void Draw(math::TransformMatrix& displayMatrix)const override;
	
	bool CanCollideWith(EntityType objectBType) override;
	void CollisionEnter(PM::Entity*) override;
	void CollisionStay(PM::Entity* obj) override;
	void CollisionExit(PM::Entity*)override;
	std::list<PM::Entity*>& getCollidingObj() { return collidingObj; }
	EntityType GetEntityType() const override { return type; }
	std::string GetEntityTypeName() const override { return "Basic_NPC"; }
	
	std::list<PM::Component*>& getTaskAIs() { return TaskAIComponents; }
	const std::vector<int> getAroundChunk();
	glm::vec3 getCheckPos() { return checkPos; }
	int getMarkState() { return MarkState; }
	void setMarkState(int state);
	double getGaze() { return suspicionGaze; }
	void setGaze(double g) { suspicionGaze = g; }
	void updateGaze(double ag) { suspicionGaze += ag; }
	unsigned int getTalkResult() { return talkResult; }
	void setTalkResult(unsigned int result) { talkResult = result; }

	bool chasing(PM::GameObject* target, double, double dt);
	bool canSeePlayer();
	bool seeing();
	bool sameRoomCheck(PM::GameObject*, PM::GameObject* B = nullptr);
	void checkPosition(glm::vec3 pos) { checkPos = pos; }
	void go(glm::vec2, double);
	bool FindRoute(std::vector<std::vector<int>> world, int nowPos,int destination,std::vector<int>& route);
	
	const double GazingSpeed(double dt);

	void setSight(glm::vec3 s);
	void setAI(PM::Component*);
	void AddTaskAI(PM::Component* Task);
	void AddImportantTaskAI(PM::Component* Task);
	bool getHasImportantTask() { return hasImportantTask; }
	PM::Component* getSelectedTask();
	void removeTask(PM::Component* task);
	bool isNeedNewTask();
	void AddDropItem(PM::GameObject* item);
	void DropItems();
	bool getIsControlled() { return isControlled; }
	bool IsDetactSound() { return SoundDetact != 0.; }

	void removeMe();
};

class NPC_generator {
	PM::GameObject* player{};
	EntityType type{ EntityType::Basic_NPC};
	NPCLevelList level{NPCLevelList::Normal};
	bool isControlled = true;
public:
	NPC_generator(PM::GameObject* p);
	void setting(EntityType t, NPCLevelList l, bool isControlled_);
	void settingType(EntityType t);
	void settingLevel(NPCLevelList l);
	void settingIsControlled(bool isControlled_);
	Basic_NPC* generate(std::string name, glm::vec2 position/*, std::initializer_list<PM::Component*> taskAIs*/);

};