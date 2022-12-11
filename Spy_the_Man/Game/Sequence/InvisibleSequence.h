#pragma once
/*--------------------------------------------------------------
File Name: InvisibleSequence.h
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 3/15/2021
-----------------------------------------------------------------*/
#include "../../Engine/FSM_base.h"
#include "../../Engine/Sequence/Sequence_base.h"

class InvisibleSequence : public PM::Sequence_base, public PM::FSM_base<InvisibleSequence> {
public:
	InvisibleSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;
private:
	class State_Invisible : public State {
	public:
		void Enter(InvisibleSequence*) override;
		void Update(InvisibleSequence*, double) override;
		void TestForExit(InvisibleSequence*) override;
		std::string GetName() override { return "State_Invisible"; }
	};
	State_Invisible stateInvisible{};
};