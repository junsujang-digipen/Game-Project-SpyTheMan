#pragma once
/*--------------------------------------------------------------
File Name: FlashbombSequence.h
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 4/25/2022
-----------------------------------------------------------------*/
#include "../../Engine/FSM_base.h"
#include "../../Engine/Sequence/Sequence_base.h"
#include "../Object/Flashbomb.h"

class FlashbombSequence : public PM::Sequence_base, public PM::FSM_base<FlashbombSequence> {
public:
	FlashbombSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;
private:
	Flashbomb* flashbombptr{nullptr};
	class State_Flashbomb : public State {
	public:
		void Enter(FlashbombSequence*) override;
		void Update(FlashbombSequence*, double) override;
		void TestForExit(FlashbombSequence*) override;
		std::string GetName() override { return "State_Dash"; }
	};
	State_Flashbomb stateFlashbomb{};
};