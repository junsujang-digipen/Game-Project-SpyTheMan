#pragma once
/*--------------------------------------------------------------
File Name: DashSequence.h
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 4/24/2022
-----------------------------------------------------------------*/
#include "../../Engine/FSM_base.h"
#include "../../Engine/Sequence/Sequence_base.h"

class DashSequence : public PM::Sequence_base, public PM::FSM_base<DashSequence> {
public:
	DashSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;
private:
	class State_Dash : public State {
	public:
		void Enter(DashSequence*) override;
		void Update(DashSequence*, double) override;
		void TestForExit(DashSequence*) override;
		std::string GetName() override { return "State_Dash"; }
	};
	State_Dash stateDash{};
};