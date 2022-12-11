/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: SpeedUp_Sequence.h
Project: GAM200
Author: Dong-A Choi
-----------------------------------------------------------------*/
#pragma once

#include "../Engine/Sequence/Sequence_base.h"
#include "../Engine/FSM_base.h"
#include "../Engine/Input.h"

class SpeedUp_Sequence : public PM::Sequence_base, public PM::FSM_base<SpeedUp_Sequence>
{
public:
	SpeedUp_Sequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* nothing);
	void Update(double dt) override;
private:
	class State_ToggleOn : public State
	{
	public:
		void Enter(SpeedUp_Sequence* speed_up) override;
		void Update(SpeedUp_Sequence* speed_up, double dt) override;
		void TestForExit(SpeedUp_Sequence* speed_up) override;
		std::string GetName() override { return "Walk"; }
	};
	class State_ToggleOff : public State {
	public:
		void Enter(SpeedUp_Sequence* speed_up) override;
		void Update(SpeedUp_Sequence* speed_up, double dt) override;
		void TestForExit(SpeedUp_Sequence* speed_up) override;
		std::string GetName() override { return "Buy"; }
	};
	State_ToggleOn state_toggle_on;
	State_ToggleOff state_toggle_off;

	bool toggled;
	PM::InputKey toggleKey;

};