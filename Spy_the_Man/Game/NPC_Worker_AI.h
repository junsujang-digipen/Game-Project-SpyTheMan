#pragma once
/*--------------------------------------------------------------
File Name: NPC_Worker_AI.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 12/7/2021
-----------------------------------------------------------------*/
#include "../Engine/AI.h"


class NPC_Worker_AI :public PM::AI<NPC_Worker_AI> {
private:
	PM::Component* taskAI = nullptr;
	class State_Wandering :public State {
	public:
		void Enter(NPC_Worker_AI*) override;
		void Update(NPC_Worker_AI*, double) override;
		void TestForExit(NPC_Worker_AI*) override;
		std::string GetName() override { return "State_Wandering"; }

	};
	class State_Suspicion :public State {

	public:
		void Enter(NPC_Worker_AI*) override;
		void Update(NPC_Worker_AI*, double) override;
		void TestForExit(NPC_Worker_AI*) override;
		std::string GetName() override { return "State_Suspicion"; }

	};
	class State_Additional_Suspicion :public State {

	public:
		void Enter(NPC_Worker_AI*) override;
		void Update(NPC_Worker_AI*, double) override;
		void TestForExit(NPC_Worker_AI*) override;
		std::string GetName() override { return "State_Additional_Suspicion"; }

	};
	class State_RunAway :public State {
		const double timerMax{7.};
		double timer{};
	public:
		void Enter(NPC_Worker_AI*) override;
		void Update(NPC_Worker_AI*, double) override;
		void TestForExit(NPC_Worker_AI*) override;
		std::string GetName() override { return "State_Attack"; }

	};
	State_Wandering stateWandering;
	State_Suspicion stateSuspicion;
	State_Additional_Suspicion stateAdditionalSuspicion;
	State_RunAway stateRunAway;


protected:

	virtual void taskSelectAlgo(double dt);

public:
	void start()override;
	NPC_Worker_AI(PM::GameObject* AIOwner);
	~NPC_Worker_AI()override { }
	//void Update(double)override;

};