#pragma once
/*--------------------------------------------------------------
File Name: ChameleonSequence.h
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 3/15/2021
-----------------------------------------------------------------*/
#include "../../Engine/FSM_base.h"
#include "../../Engine/Sequence/Sequence_base.h"

class ChameleonSequence : public PM::Sequence_base, public PM::FSM_base<ChameleonSequence> {
public:
	ChameleonSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;
private:
	class State_Chameleon : public State {
	public:
		void Enter(ChameleonSequence*) override;
		void Update(ChameleonSequence*, double) override;
		void TestForExit(ChameleonSequence*) override;
		std::string GetName() override { return "State_Chameleon"; }
	};
	State_Chameleon stateChameleon{};
};