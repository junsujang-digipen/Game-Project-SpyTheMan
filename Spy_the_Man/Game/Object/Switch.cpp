/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/

#include "Switch.h"


#include "Item.h"
#include "../Sequence/GetItemSequence.h"
#include "../UI/SkillButton.h"
#include "../Game/Object/ButtonManager.h"
#include "../Lazer.h"


class SwitchSequence : public PM::Sequence_base, public PM::FSM_base<SwitchSequence> {

	class State_going : public State {
	public:
		void Enter(SwitchSequence*) override;
		void Update(SwitchSequence*, double) override;
		void TestForExit(SwitchSequence*) override;
		std::string GetName() override { return "State_going"; }
	};

	State_going stateGoing{};

public:
	SwitchSequence(PM::SequenceType type, PM::GameObject*, PM::GameObject*);
	void Update(double dt)override;

};


SwitchSequence::SwitchSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* obj)
	: Sequence_base(type, player, obj)
{
	ChangeState(&stateGoing);
}
void SwitchSequence::Update(double dt)
{
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void SwitchSequence::State_going::Enter(SwitchSequence* sq)
{
	static_cast<Player*>(sq->obj1)->GoTo({ sq->obj2->GetPosition().x + sq->obj2->GetSize().x / 2 , sq->obj1->GetPosition().y });
}

void SwitchSequence::State_going::Update(SwitchSequence*, double)
{

}


void SwitchSequence::State_going::TestForExit(SwitchSequence* sq)
{
	Player* player = static_cast<Player*>(sq->obj1);
	PM::GameObject* obj = sq->obj2;

	if (player->Isinterrupted() || obj->Isinterrupted())
	{
		player->SetGoTo(false);
		player->SetInterrupted(false);
		obj->SetInterrupted(false);
		sq->setIsValid(false);
	}
	else if (player->GetGoToPos() == false)
	{
		auto a = dynamic_cast<Switch*>(obj);

		if(a->GetIsOn() == true)
		{
			a->Off();
		}else
		{
			a->On();
		}
		sq->setIsValid(false);
	}
}



Switch::Switch(std::string name, glm::vec2 pos, glm::vec2 size , std::string filepath) : GameObject(name, pos, size, filepath)
{
	GetGOComponent<ButtonManager>()->AddButton(new SequenceButton<SwitchSequence>(
		"Switch", this, "assets/UI/Button/power.png"));
	currState = &state_nothing;
	currState->Enter(this);
	AddGOComponent(new PM::RectCollision({{0,0},size},this));
}

void Switch::On()
{
	IsOn = true;
	ChangeImage("assets/GameObject/Switch_On.png");
}

void Switch::Off()
{
	IsOn = false;
	ChangeImage("assets/GameObject/Switch_Off.png");
}

void Switch::CollisionEnter(Entity*)
{
	Off();
}


LaserSwitch::LaserSwitch(std::string name, glm::vec2 pos) : Switch(name,pos)
{}

void LaserSwitch::On()
{
	Switch::On();
	for (auto& obj : Engine::GetGSComponent<PM::World>()->GetObjects())
	{
		if (obj->GetEntityType() == EntityType::Lazer)
		{
			dynamic_cast<Lazer*>(obj)->SetPowerOff(false);
		}
	}
}

void LaserSwitch::Off()
{
	Switch::Off();
	for(auto& obj : Engine::GetGSComponent<PM::World>()->GetObjects())
	{
		if(obj->GetEntityType() == EntityType::Lazer)
		{
			dynamic_cast<Lazer*>(obj)->SetPowerOff(true);
		}
	}
}

bool Switch::CanCollideWith(EntityType objectBType)
{
	return objectBType == EntityType::Player;
}
