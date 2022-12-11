#pragma once
/*--------------------------------------------------------------
File Name: Task_AI_pointToPoint.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/11/2021
-----------------------------------------------------------------*/
#include "../Engine/AI.h"

class Task_AI_pointToPoint :public PM::AI<Task_AI_pointToPoint> {
	glm::vec2 targetPosition{};
	const double returnSpeed{ 200 };
	glm::vec2 firstP{};
	glm::vec2 secondP{};

	bool precisePosition{false};
	const double newTaskTimeLimit{6.};
	double newTaskTime{};

	class State_going :public State {
		bool whatPoint {false};
		void switchPoint() { whatPoint = !whatPoint; }

	public:
		void Enter(Task_AI_pointToPoint*) override;
		void Update(Task_AI_pointToPoint*, double) override;
		void TestForExit(Task_AI_pointToPoint*) override;
		std::string GetName() override { return "State_going"; }
	};
	class State_working :public State {
		const double TimeLimit{ 3. };
		double nowT{};

	public:
		void Enter(Task_AI_pointToPoint*) override;
		void Update(Task_AI_pointToPoint*, double) override;
		void TestForExit(Task_AI_pointToPoint*) override;
		std::string GetName() override { return "State_working"; }
	};
	State_going stateGoing;
	State_working stateWorking;
public:
	Task_AI_pointToPoint(PM::GameObject* AIOwner, glm::vec2 point1 = {}, glm::vec2 point2 = {},bool pp = true );
	Task_AI_pointToPoint(PM::GameObject* newOwner, Task_AI_pointToPoint& rhs);
	~Task_AI_pointToPoint() {}
	void start()override;
};
