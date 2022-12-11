/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/

#include "Key.h"

#include "Item.h"
#include "../Sequence/GetItemSequence.h"
#include "../UI/SkillButton.h"
#include "../Game/Object/ButtonManager.h"

Key::Key(std::string name, glm::vec2 pos, std::string targetName, glm::vec2 size , std::string filepath) :GameObject(name, pos, size, filepath)
{
	AddGOComponent(new KeyItem(targetName));
	GetGOComponent<ButtonManager>()->AddButton(new SequenceButton<GetItemSequence>(
		"GetItem", this, "assets/GameObject/UI/Get.png"));
	AddGOComponent(new KeyItem(targetName));
	currState = &state_nothing;
	currState->Enter(this);
}

void Key::Update(double dt) {
	GameObject::Update(dt);
	PM::World* world = Engine::GetGSComponent<PM::World>();
	Player* playerptr = dynamic_cast<Player*>(world->GetPlayerPtr());
	if (playerptr->IsVent() == true) {
		GetGOComponent<ButtonManager>()->GetButton("GetItem")->SetLock(true);
	}
	else {
		GetGOComponent<ButtonManager>()->GetButton("GetItem")->SetLock(false);
	}
}
