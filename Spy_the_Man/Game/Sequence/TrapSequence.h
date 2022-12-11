#pragma once
/*--------------------------------------------------------------
File Name: TrapSequence.h
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 4/24/2022
-----------------------------------------------------------------*/
#include "../../Engine/FSM_base.h"
#include "../../Engine/Sequence/Sequence_base.h"
#include "../Object/Trap.h"

class TrapSequence : public PM::Sequence_base, public PM::FSM_base<TrapSequence> {
public:
	TrapSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;
private:
	Trap* trapptr{nullptr};
	bool left = false;
	bool select = false;
	class State_Trap : public State {
	public:
		void Enter(TrapSequence*) override;
		void Update(TrapSequence*, double) override;
		void TestForExit(TrapSequence*) override;
		std::string GetName() override { return "State_Trap"; }
	};
	State_Trap stateTrap{};
};