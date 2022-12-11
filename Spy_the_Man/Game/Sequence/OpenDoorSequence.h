#pragma once
/*--------------------------------------------------------------
File Name: OpenDoorSequence.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/9/2021
-----------------------------------------------------------------*/
#include "../../Engine/Sequence/Sequence_base.h"
#include "../../Engine/FSM_base.h"

class OpenDoorSequence :public PM::Sequence_base, public PM::FSM_base<OpenDoorSequence> {

	class State_going : public State {
	public:
		void Enter(OpenDoorSequence*) override;
		void Update(OpenDoorSequence*, double) override;
		void TestForExit(OpenDoorSequence*) override;
		std::string GetName() override { return "State_going_to_door"; }
	};
	class State_openDoor : public State {
	public:
		void Enter(OpenDoorSequence*) override;
		void Update(OpenDoorSequence*, double) override;
		void TestForExit(OpenDoorSequence*) override;
		std::string GetName() override { return "State_openDoor"; }
	};
	State_going stateGoing{};
	State_openDoor stateOpenDoor{};
public:
	OpenDoorSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;
};