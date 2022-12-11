#pragma once
/*--------------------------------------------------------------
File Name: OpenDoorSequence.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/9/2021
-----------------------------------------------------------------*/
#include "../../Engine/Sequence/Sequence_base.h"
#include "../../Engine/FSM_base.h"

class HideSequence :public PM::Sequence_base, public PM::FSM_base<HideSequence> {

	class State_going : public State {
	public:
		void Enter(HideSequence*) override;
		void Update(HideSequence*, double) override;
		void TestForExit(HideSequence*) override;
		std::string GetName() override { return "State_going_to_door"; }
	};
	class State_Hiding : public State {
	public:
		void Enter(HideSequence*) override;
		void Update(HideSequence*, double) override;
		void TestForExit(HideSequence*) override;
		std::string GetName() override { return "State_openDoor"; }
	};
	State_going stateGoing{};
	State_Hiding stateHiding{};
public:
	HideSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;
};