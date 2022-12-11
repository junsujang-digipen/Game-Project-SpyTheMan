#pragma once
/*--------------------------------------------------------------
File Name: NPC_OpenDoorSequence.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/10/2021
-----------------------------------------------------------------*/
#include "../../Engine/Sequence/Sequence_base.h"
#include "../../Engine/FSM_base.h"

class NPC_OpenDoorSequence :public PM::Sequence_base, public PM::FSM_base<NPC_OpenDoorSequence> {
	class State_openDoor : public State {
	public:
		void Enter(NPC_OpenDoorSequence*) override;
		void Update(NPC_OpenDoorSequence*, double) override;
		void TestForExit(NPC_OpenDoorSequence*) override;
		std::string GetName() override { return "State_openDoor"; }
	};
	State_openDoor stateOpenDoor{};
public:
	NPC_OpenDoorSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;
};

class NPC_CloseDoorSequence :public PM::Sequence_base, public PM::FSM_base<NPC_CloseDoorSequence> {
	class State_closeDoor : public State {
	public:
		void Enter(NPC_CloseDoorSequence*) override;
		void Update(NPC_CloseDoorSequence*, double) override;
		void TestForExit(NPC_CloseDoorSequence*) override;
		std::string GetName() override { return "State_openDoor"; }
	};
	State_closeDoor stateCloseDoor{};
public:
	NPC_CloseDoorSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;
};