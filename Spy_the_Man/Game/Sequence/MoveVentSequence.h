#pragma once
/*--------------------------------------------------------------
File Name: MoveStairSequence.h
Project: Spy The Man
Author: Seongwon Jang
Creation date: 11/10/2021
-----------------------------------------------------------------*/
#include "../../Engine/Sequence/Sequence_base.h"
#include "../../Engine/FSM_base.h"

class MoveVentSequence :public PM::Sequence_base, public PM::FSM_base<MoveVentSequence> {

	class State_going : public State {
	public:
		void Enter(MoveVentSequence*) override;
		void Update(MoveVentSequence*, double) override;
		void TestForExit(MoveVentSequence*) override;
		std::string GetName() override { return "State_going_to_door"; }
	};
	class State_MoveVent : public State {
	public:
		void Enter(MoveVentSequence*) override;
		void Update(MoveVentSequence*, double) override;
		void TestForExit(MoveVentSequence*) override;
		std::string GetName() override { return "State_openDoor"; }
	};

	State_going stateGoing{};
	State_MoveVent stateMoveVent{};
public:
	MoveVentSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;
};