/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Kill_AI.h
Project: CS200
Author: Dong-A Choi
-----------------------------------------------------------------*/
#pragma once
#include "../../Engine/Sequence/Sequence_base.h"
#include "../../Engine/FSM_base.h"

class Kill_AI : public PM::Sequence_base, public PM::FSM_base<Kill_AI>
{
public:
	Kill_AI(PM::SequenceType type, PM::GameObject* player, PM::GameObject* enemy);
	void Update(double dt) override;
private:
	class State_walking : public State {
	public:
		void Enter(Kill_AI* kill) override;
		void Update(Kill_AI* kill, double dt) override;
		void TestForExit(Kill_AI* kill) override;
		std::string GetName() override { return "Walk"; }
	};
	class State_kill : public State {
	public:
		void Enter(Kill_AI* kill) override;
		void Update(Kill_AI* kill, double dt) override;
		void TestForExit(Kill_AI* kill) override;
		std::string GetName() override { return "Kill"; }
	};
	State_walking state_walking_;
	State_kill state_kill_;

	bool enemyDeath = false;
	double timer = 0;
};