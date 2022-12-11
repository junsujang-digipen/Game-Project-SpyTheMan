#pragma once
/*--------------------------------------------------------------
File Name: SilentRunSequence.h
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 4/13/2021
-----------------------------------------------------------------*/
#include "../../Engine/FSM_base.h"
#include "../../Engine/Sequence/Sequence_base.h"

class SilentRunSequence : public PM::Sequence_base, public PM::FSM_base<SilentRunSequence> {
public:
	SilentRunSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;
private:
	class State_SilentRun : public State {
	public:
		void Enter(SilentRunSequence*) override;
		void Update(SilentRunSequence*, double) override;
		void TestForExit(SilentRunSequence*) override;
		std::string GetName() override { return "State_SilentRun"; }
	};
	State_SilentRun stateSilentRun{};
};