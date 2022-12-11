#pragma once
/*--------------------------------------------------------------
File Name: PushSequence.h
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 4/26/2022
-----------------------------------------------------------------*/
#include "../../Engine/FSM_base.h"
#include "../../Engine/Sequence/Sequence_base.h"
#include "../Object/Push.h"

class PushSequence : public PM::Sequence_base, public PM::FSM_base<PushSequence> {
public:
	PushSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;
private:
	Push* pushptr{ nullptr };
	bool left = false;
	bool select = false;
	class State_Push : public State {
	public:
		void Enter(PushSequence*) override;
		void Update(PushSequence*, double) override;
		void TestForExit(PushSequence*) override;
		std::string GetName() override { return "State_Trap"; }
	};
	State_Push statePush{};
};