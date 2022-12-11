/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Project: Spy the Man
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include "Vent.h"

#include "Wall.h"
#include "../Player.h"
#include "../../Engine/Engine.h"
#include "../Engine/Declaration.h"

#include "../Sequence/MoveVentSequence.h"
#include "../UI/SkillButton.h"
#include "../Game/Object/ButtonManager.h"
VentDoor::VentDoor(std::string name, glm::vec2 pos, std::string targetName, glm::vec2 size, std::string filepath) :
GameObject(name,pos, size, filepath), TargetName(targetName), TargetPtr(nullptr)
{
	GetGOComponent<ButtonManager>()->AddButton(new SequenceButton<MoveVentSequence>(
		"MoveVent", this, "assets/GameObject/UI/Vent.png"));
}

VentDoor::~VentDoor() {}

void VentDoor::Update(double dt)
{
	Player* PlayerPtr = dynamic_cast<Player*>(Engine::GetGSComponent<PM::World>()->GetPlayerPtr());
	GameObject* target = GetTarget();
	if ( GetPosition().y < target->GetPosition().y)
	{
		if (PlayerPtr->IsVent() == true)
		{
			GetGOComponent<ButtonManager>()->GetButton("MoveVent")->SetLock(true);
		}
		else
		{
			GetGOComponent<ButtonManager>()->GetButton("MoveVent")->SetLock(false);
		}
	}
	else
	{
		if (PlayerPtr->IsVent() == false)
		{
			GetGOComponent<ButtonManager>()->GetButton("MoveVent")->SetLock(true);
		}
		else
		{
			GetGOComponent<ButtonManager>()->GetButton("MoveVent")->SetLock(false);
		}
	}
	GameObject::Update(dt);
}


PM::GameObject* VentDoor::GetTarget()
{
	if (TargetPtr == nullptr)
	{
		Engine::GetLogger().LogDebug("Vent need to Find Target");
		auto objs = Engine::GetGSComponent<PM::World>()->GetObjects();
		for (auto obj : objs)
		{
			if (TargetName == obj->GetName())
			{
				TargetPtr = obj;
				break;
			}
		}
		if (TargetPtr == nullptr)
		{
			Engine::GetLogger().LogError("Stair Can't Find the Target Vent. Check Target Vent Name: " + TargetName + '"');
		}
	}

	return TargetPtr;
}




EntityType VentDoor::GetEntityType() const
{
	return EntityType::Vent_obj;
}

std::string VentDoor::GetEntityTypeName() const
{
	return "VentDoor";
}


VentPipe::VentPipe(std::string name, double Xpos, double Xsize, double floor) : GameObject(name,{Xpos,ROOMYSIZE-125 - WALLTHICKNESS + ROOMYSIZE*floor})
{
	SetSize({ Xsize , 125 });
	currState = &state_nothing;
	currState->Enter(this);
	glm::vec2 pos = GetPosition();
	Engine::GetGSComponent<PM::World>()->Add(new Wall(GetName() + "Lwall", {pos.x- WALLTHICKNESS,pos.y}, { WALLTHICKNESS,125 }), this->GetLayer());
	Engine::GetGSComponent<PM::World>()->Add(new Wall(GetName() + "Rwall", {pos.x+GetSize().x,pos.y}, { WALLTHICKNESS,125 }), this->GetLayer());
}
VentPipe::VentPipe(std::string name, glm::vec2 position, double xsize) : GameObject(name, { position.x,ROOMYSIZE - 125 - WALLTHICKNESS + ROOMYSIZE * position.y })
{
	SetSize({ xsize , 125 });
	currState = &state_nothing;
	currState->Enter(this);
	glm::vec2 pos = GetPosition();
	pos.y = GetPosition().y;
	Engine::GetGSComponent<PM::World>()->Add(new Wall(GetName() + "Lwall", { pos.x - WALLTHICKNESS,pos.y  }, { WALLTHICKNESS,125 }), this->GetLayer());
	Engine::GetGSComponent<PM::World>()->Add(new Wall(GetName() + "Rwall", { pos.x + GetSize().x,pos.y }, { WALLTHICKNESS,125 }), this->GetLayer());
	Engine::GetGSComponent<PM::World>()->Add(new VentDoor(GetName() + "LDOWN", { pos.x + 200,pos.y  - 125 - WALLTHICKNESS }, GetName() + "LUP"));
	Engine::GetGSComponent<PM::World>()->Add(new VentDoor(GetName() + "RDOWN", { pos.x + xsize - 125 - 200,pos.y  - 125 - WALLTHICKNESS }, GetName() + "RUP"));
	Engine::GetGSComponent<PM::World>()->Add(new VentDoor(GetName() + "LUP", { pos.x + 200,pos.y   }, GetName() + "LDOWN"));
	Engine::GetGSComponent<PM::World>()->Add(new VentDoor(GetName() + "RUP", { pos.x + xsize - 125 - 200,pos.y  }, GetName() + "RDOWN"));
}
VentPipe::~VentPipe()
{
}

void VentPipe::Update(double dt)
{
	
	GameObject::Update(dt);
}

void VentPipe::Draw(math::TransformMatrix& displayMatrix) const
{
	PM::push_settings();
	PM::no_outline();
	PM::set_fill_color(100, 100, 100);
	PM::rect_corner();
	PM::apply_matrix(displayMatrix.matrix);
	glm::vec2 Size = GetSize();
	if(dynamic_cast<Player*>(Engine::GetGSComponent<PM::World>()->GetPlayerPtr())->IsVent() == true)
	{
		PM::set_fill_color(200, 200, 200, 155);
	}
	PM::draw_rect(GetPosition().x, GetPosition().y, Size.x, Size.y);
	PM::pop_settings();
}

EntityType VentPipe::GetEntityType() const
{
	return EntityType::Vent_obj;
}

std::string VentPipe::GetEntityTypeName() const
{
	return "VentPipe";
}

bool VentDoor::CanCollideWith(EntityType objectBType)
{
	return objectBType == EntityType::Player;
}

void VentDoor::CollisionStay(Entity* obj)
{
	new MoveVentSequence(PM::SequenceType::Player, dynamic_cast<Player*>(obj), this);
}
