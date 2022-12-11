#pragma once
/*--------------------------------------------------------------
File Name: MoveStairSequence.h
Project: Spy The Man
Author: Seongwon Jang
Creation date: 11/10/2021
-----------------------------------------------------------------*/
#include "../../Engine/Sequence/Sequence_base.h"
#include "../../Engine/FSM_base.h"

class MoveStairSequence :public PM::Sequence_base, public PM::FSM_base<MoveStairSequence> {

	class State_going : public State {
	public:
		void Enter(MoveStairSequence*) override;
		void Update(MoveStairSequence*, double) override;
		void TestForExit(MoveStairSequence*) override;
		std::string GetName() override { return "State_going_to_door"; }
	};
	class State_MoveStair : public State {
		double time{};
	public:
		void Enter(MoveStairSequence*) override;
		void Update(MoveStairSequence*, double) override;
		void TestForExit(MoveStairSequence*) override;
		std::string GetName() override { return "State_openDoor"; }
	};
	
	State_going stateGoing{};
	State_MoveStair stateMoveStair{};
public:
	MoveStairSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;
};