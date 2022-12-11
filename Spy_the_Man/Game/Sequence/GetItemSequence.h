#pragma once
#include <list>

#include "../Engine/FSM_base.h"
#include "../Engine/Sequence/Sequence_base.h"

class GetItemSequence : public PM::Sequence_base, public PM::FSM_base<GetItemSequence> {

	class State_going : public State {
	public:
		void Enter(GetItemSequence*) override;
		void Update(GetItemSequence*, double) override;
		void TestForExit(GetItemSequence*) override;
		std::string GetName() override { return "State_going"; }
	};

	State_going stateGoing{};

public:
	GetItemSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;

};
