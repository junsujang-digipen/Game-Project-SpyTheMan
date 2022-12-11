#pragma once
/*--------------------------------------------------------------
File Name: VisualSequence.h
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 3/15/2021
-----------------------------------------------------------------*/
#include "../../Engine/FSM_base.h"
#include "../../Engine/Sequence/Sequence_base.h"

class VisualSequence : public PM::Sequence_base, public PM::FSM_base<VisualSequence> {
public:
	VisualSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;
private:
	class State_Visual : public State {
	public:
		void Enter(VisualSequence*) override;
		void Update(VisualSequence*, double) override;
		void TestForExit(VisualSequence*) override;
		std::string GetName() override { return "State_Visualize"; }
	};
	State_Visual stateVisual{};
};