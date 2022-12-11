#pragma once
#include "../../Engine/Component.h"
#include "../../Engine/Engine.h"
#include "../../Engine/World.h"
#include "../Player.h"

class Item : public PM::Component
{
public:
	Item() :Component("Item") {}
	virtual void GetItem() = 0;
};

class ClothesItem : public Item
{
public:
	ClothesItem(DisguiseList d) : Item(), dis(d) {};
	void GetItem() override { static_cast<Player*>(Engine::GetGSComponent<PM::World>()->GetPlayerPtr())->SetDisguise(dis); }

private:
	DisguiseList dis;
};

class KeyItem : public Item
{
public:
	KeyItem(std::string target) :TargetName(target) {}
	
	void GetItem() override {
		auto objs = Engine::GetGSComponent<PM::World>()->GetObjects();
		for (auto obj : objs)
		{
			if (TargetName == obj->GetName())
			{
				static_cast<SideDoor*>(obj)->UnLock();
				break;
			}
		}
	}

private:
	std::string TargetName;
};

