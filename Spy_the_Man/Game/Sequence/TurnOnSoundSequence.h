/*--------------------------------------------------------------
File Name: TurnOnSoundSequence.h
Project: Spy The Man
Author: Dong-A Choi
Creation date: 11/27/2021
-----------------------------------------------------------------*/
#pragma once
#include "../../Engine/FSM_base.h"
#include "../../Engine/Sequence/Sequence_base.h"

class TurnOnSoundSequence :public PM::Sequence_base, public PM::FSM_base<TurnOnSoundSequence> {

	class State_going : public State {
	public:
		void Enter(TurnOnSoundSequence*) override;
		void Update(TurnOnSoundSequence*, double) override;
		void TestForExit(TurnOnSoundSequence*) override;
		std::string GetName() override { return "State_going"; }
	};

	State_going stateGoing{};
public:
	TurnOnSoundSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*, std::string soundName,int soundSize);
	void Update(double dt)override;
};