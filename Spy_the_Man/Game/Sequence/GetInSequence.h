/*--------------------------------------------------------------
File Name: GetInSequence.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/27/2021
-----------------------------------------------------------------*/
#pragma once
#include "../../Engine/FSM_base.h"
#include "../../Engine/Sequence/Sequence_base.h"


class GetInSequence :public PM::Sequence_base, public PM::FSM_base<GetInSequence> {

	class State_going : public State {
	public:
		void Enter(GetInSequence*) override;
		void Update(GetInSequence*, double) override;
		void TestForExit(GetInSequence*) override;
		std::string GetName() override { return "GetInSequence"; }
	};

	State_going stateGoing{};

public:
	GetInSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;
};
