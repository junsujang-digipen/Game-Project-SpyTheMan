#pragma once
/*--------------------------------------------------------------
File Name: FindSequence.h
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 4/29/2021
-----------------------------------------------------------------*/
#include "../../Engine/FSM_base.h"
#include "../../Engine/Sequence/Sequence_base.h"
#include "../Object/Find.h"

class FindSequence : public PM::Sequence_base, public PM::FSM_base<FindSequence> {
public:
	FindSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;
private:
	Find* find;
	class State_Find : public State {
	public:
		void Enter(FindSequence*) override;
		void Update(FindSequence*, double) override;
		void TestForExit(FindSequence*) override;
		std::string GetName() override { return "State_Find"; }
	};
	State_Find stateFind{};
};