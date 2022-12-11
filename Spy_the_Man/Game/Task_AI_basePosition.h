#pragma once
/*--------------------------------------------------------------
File Name: Task_AI_basePosition.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/2/2021
-----------------------------------------------------------------*/
#include "../Engine/AI.h"

class Task_AI_basePosition :public PM::AI<Task_AI_basePosition> {
	glm::vec2 basePosition{};
	const double returnSpeed{250};
	const double TimeLimit{5.};
	bool precisePosition{ false };
	const glm::vec3 seeingVec{};
	class State_going :public State {
	public:
		void Enter(Task_AI_basePosition*) override;
		void Update(Task_AI_basePosition*, double) override;
		void TestForExit(Task_AI_basePosition*) override;
		std::string GetName() override { return "State_going"; }
	};
	class State_working :public State {
		
		double nowT{};

	public:
		void Enter(Task_AI_basePosition*) override;
		void Update(Task_AI_basePosition*, double) override;
		void TestForExit(Task_AI_basePosition*) override;
		std::string GetName() override { return "State_working"; }
	};
	State_going stateGoing;
	State_working stateWorking;
public:
	Task_AI_basePosition(PM::GameObject* AIOwner, glm::vec2 basePosition, glm::vec3 seeingVec = {}, bool pp = true, const double tLimt = 5.);
	Task_AI_basePosition(PM::GameObject* newOwner, Task_AI_basePosition&rhs);
	void start()override;
};

