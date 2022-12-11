#pragma once
/*--------------------------------------------------------------
File Name: DroneSequence.h
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 3/13/2021
-----------------------------------------------------------------*/
#include "../../Engine/FSM_base.h"
#include "../../Engine/Sequence/Sequence_base.h"


class Drone;

namespace PM
{
	namespace UI
	{
		class Rect;
		class Button;
		class Image;
	}
}

class DroneSequence : public PM::Sequence_base, public PM::FSM_base<DroneSequence> {
public:
	DroneSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	~DroneSequence();
	void Update(double dt)override;
private:
	Drone* droneptr{ nullptr };
	class State_Begin : public State
	{
		void Enter(DroneSequence*) override;
		void Update(DroneSequence*, double) override;
		void TestForExit(DroneSequence*) override;
		std::string GetName() override { return "State_Begin"; }
	};
	class State_BlackScreen : public State
	{
		void Enter(DroneSequence*) override;
		void Update(DroneSequence*, double) override;
		void TestForExit(DroneSequence*) override;
		std::string GetName() override { return "Black_Screen"; }

		PM::UI::Rect* blackImage{nullptr};
	};
	class State_Drone : public State {
	public:
		void Enter(DroneSequence*) override;
		void Update(DroneSequence*, double) override;
		void TestForExit(DroneSequence*) override;
		std::string GetName() override { return "State_Drone"; }

		PM::UI::Rect* blackImage{nullptr};
		PM::UI::Image* DeviceImage{ nullptr };
		PM::UI::Button* destroyButton{ nullptr };
		PM::UI::Button* makeSoundButton{ nullptr };
	};
	class State_End : public State
	{
		void Enter(DroneSequence*) override;
		void Update(DroneSequence*, double) override;
		void TestForExit(DroneSequence*) override;
		std::string GetName() override { return "State_End"; }

		double deadtime = 1;
	};
	State_Begin stateBegin{};
	State_BlackScreen stateBlackScreen{};
	State_Drone stateDrone{};
	State_End stateEnd{};
};
