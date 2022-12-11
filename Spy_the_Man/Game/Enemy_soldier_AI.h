#pragma once
/*--------------------------------------------------------------
File Name: Enemy_soldier_AI.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 9/24/2021
-----------------------------------------------------------------*/
#include "../Engine/AI.h"

class CardInventory;
class Task_AI_pointToPoint;
class Enemy_soldier_AI :public PM::AI<Enemy_soldier_AI> {
private:
	PM::Component* taskAI = nullptr;
	class State_Wandering :public State {
	public:
		void Enter(Enemy_soldier_AI*) override;
		void Update(Enemy_soldier_AI*, double) override;
		void TestForExit(Enemy_soldier_AI*) override;
		std::string GetName() override { return "State_Wandering"; }

	};
	class State_Suspicion :public State {

	public:
		void Enter(Enemy_soldier_AI*) override;
		void Update(Enemy_soldier_AI*, double) override;
		void TestForExit(Enemy_soldier_AI*) override;
		std::string GetName() override { return "State_Suspicion"; }

	};
	class State_Additional_Suspicion :public State {
		
	public:
		void Enter(Enemy_soldier_AI*) override;
		void Update(Enemy_soldier_AI*, double) override;
		void TestForExit(Enemy_soldier_AI*) override;
		std::string GetName() override { return "State_Additional_Suspicion"; }

	};
	class State_Attack :public State {
	public:
		void Enter(Enemy_soldier_AI*) override;
		void Update(Enemy_soldier_AI*, double) override;
		void TestForExit(Enemy_soldier_AI*) override;
		std::string GetName() override { return "State_Attack"; }

	};
	class State_go :public State {
		glm::vec2 destination{};
	public:
		void setDest(glm::vec2);
		void Enter(Enemy_soldier_AI*) override;
		void Update(Enemy_soldier_AI*, double) override;
		void TestForExit(Enemy_soldier_AI*) override;
		std::string GetName() override { return "State_go"; }

	};
	class State_checkAround :public State {
		Task_AI_pointToPoint* checkAroundAI{nullptr};
		std::vector<int> arc{};
		int checkNum{};
		const double chaseT{ 3. };
		double chasingTime{ 0 };
	public:
		void Enter(Enemy_soldier_AI*) override;
		void Update(Enemy_soldier_AI*, double) override;
		void TestForExit(Enemy_soldier_AI*) override;
		std::string GetName() override { return "State_checkAround"; }
		void setCheckAI(Task_AI_pointToPoint* ai) { checkAroundAI = ai; }
		~State_checkAround() override;
	};
	State_Wandering stateWandering;
	State_Suspicion stateSuspicion;
	State_Additional_Suspicion stateAdditionalSuspicion;
	State_Attack stateAttack;
	State_checkAround stateCheckAround;
	State_go stateGo;
	
protected:
	
	virtual void taskSelectAlgo(double dt);

public:
	void start()override;
	Enemy_soldier_AI(PM::GameObject* AIOwner);
	~Enemy_soldier_AI()override { stateCheckAround.~State_checkAround(); }
	//void Update(double)override;

};