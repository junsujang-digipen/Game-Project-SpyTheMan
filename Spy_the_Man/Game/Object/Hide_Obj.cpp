/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/

#include "Hide_Obj.h"

#include "ButtonManager.h"
#include "../Sequence/HideSequence.h"
#include "../UI/SkillButton.h"
#include "../Player.h"

namespace PM
{
	class World;
}

Hide_Obj::Hide_Obj(std::string name, glm::vec2 pos, glm::vec2 size, std::string filepath)
	: GameObject(name,pos,size, filepath)
{
	AddGOComponent(new PM::RectCollision({{0,0},{size}}, this));
	GetGOComponent<ButtonManager>()->AddButton(new SequenceButton<HideSequence>(
		" ", this, "assets/GameObject/UI/Hide.png"));
	
}

void Hide_Obj::Update(double dt) {
	GameObject::Update(dt);
	PM::World* world = Engine::GetGSComponent<PM::World>();
	Player* playerptr = dynamic_cast<Player*>(world->GetPlayerPtr());
	if (playerptr->IsVent() == true) {
		GetGOComponent<ButtonManager>()->GetButton(" ")->SetLock(true);
	}
	else {
		GetGOComponent<ButtonManager>()->GetButton(" ")->SetLock(false);
	}
}

bool Hide_Obj::CanCollideWith(EntityType objectBType)
{
	return objectBType == EntityType::Player;
}


void Hide_Obj::CollisionStay(Entity*)
{
	if (Engine::GetInput().IsKeyReleased(PM::InputKey::Keyboard::W)) {
		new HideSequence(PM::SequenceType::Player, Engine::GetGSComponent<PM::World>()->GetPlayerPtr(), this);
	}
}