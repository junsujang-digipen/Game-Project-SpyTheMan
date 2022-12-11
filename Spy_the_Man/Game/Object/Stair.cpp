/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Project: Spy the Man
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include "Stair.h"


#include "../../Engine/Engine.h"
#include "../Sequence/MoveStairSequence.h"
#include "../UI/SkillButton.h"
#include "../Game/Object/ButtonManager.h"
#include "../Player.h"

Stair::Stair(std::string name, glm::vec2 pos, std::string targetName, bool isDown, glm::vec2 size, std::string filepath)
	: GameObject(name, pos, size, filepath), TargetName(targetName), TargetStair(nullptr)
{
	AddGOComponent(new PM::RectCollision({{0,0},size}, this));
	if (isDown)
	{
		GetGOComponent<ButtonManager>()->AddButton(new SequenceButton<MoveStairSequence>(
			"MoveStair", this, "assets/UI/Button/Down.png"));
	}
	else
	{
		GetGOComponent<ButtonManager>()->AddButton(new SequenceButton<MoveStairSequence>(
			"MoveStair", this, "assets/UI/Button/Up.png"));
	}
}

void Stair::CollisionStay(Entity*)
{
	if (Engine::GetInput().IsKeyReleased(PM::InputKey::Keyboard::W)) {
		new MoveStairSequence(PM::SequenceType::Player, Engine::GetGSComponent<PM::World>()->GetPlayerPtr(), this);
	}
}


PM::GameObject* Stair::GetTarget() const
{
	if(TargetStair == nullptr)
	{
		Engine::GetLogger().LogDebug("Stair need to Find Target");
		auto objs = Engine::GetGSComponent<PM::World>()->GetObjects();
		for (auto obj : objs)
		{
			if (TargetName == obj->GetName())
			{
				TargetStair = obj;
				break;
			}
		}
		if (TargetStair == nullptr)
		{
			Engine::GetLogger().LogError("Stair Can't Find the Target Stair. Check Target Stair Name: " + TargetName + '"');
		}
	}

	return TargetStair;
}

void Stair::Update(double dt) {
	GameObject::Update(dt);
	PM::World* world = Engine::GetGSComponent<PM::World>();
	Player* playerptr = dynamic_cast<Player*>(world->GetPlayerPtr());
	if (playerptr->IsVent() == true) {
		GetGOComponent<ButtonManager>()->GetButton("MoveStair")->SetLock(true);
	}
	else {
		GetGOComponent<ButtonManager>()->GetButton("MoveStair")->SetLock(false);
	}
}
