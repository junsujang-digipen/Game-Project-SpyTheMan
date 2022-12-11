/*--------------------------------------------------------------
File Name: Enemy_soldier_AI.cpp
Project: Spy The Man
Author: Seongwon Jang
Creation date: 9/24/2021
-----------------------------------------------------------------*/
#include "Clothes.h"
#include "Item.h"
#include "../Sequence/GetItemSequence.h"
#include "../UI/SkillButton.h"
#include "../Object/ButtonManager.h"

Clothes::Clothes(std::string name, glm::vec2 pos, DisguiseList d, glm::vec2 size, std::string filepath)
	:GameObject(name,pos,size,filepath)
{
	AddGOComponent(new ClothesItem(d));
	GetGOComponent<ButtonManager>()->AddButton(new SequenceButton<GetItemSequence>(
		" ",this, "assets/GameObject/UI/Disguise.png"));
	AddGOComponent(new ClothesItem(d));
}

void Clothes::Update(double dt) {
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

Clothes::~Clothes() {}


