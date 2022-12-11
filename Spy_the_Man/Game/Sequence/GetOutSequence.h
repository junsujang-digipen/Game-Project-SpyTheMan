/*--------------------------------------------------------------
File Name: GetOutSequence.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/27/2021
-----------------------------------------------------------------*/
#pragma once
#include "../../Engine/FSM_base.h"
#include "../../Engine/Sequence/Sequence_base.h"

class GetOutSequence :public PM::Sequence_base, public PM::FSM_base<GetOutSequence> {

	class State_going : public State {
	public:
		void Enter(GetOutSequence*) override;
		void Update(GetOutSequence*, double) override;
		void TestForExit(GetOutSequence*) override;
		std::string GetName() override { return "State_going_to_door"; }
	};

	State_going stateGoing{};

public:
	GetOutSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;
};
