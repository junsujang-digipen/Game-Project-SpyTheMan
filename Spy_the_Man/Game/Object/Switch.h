/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/

#pragma once
#include "../../Engine/GameObject.h"
#include<glm/glm.hpp>

#include "../../Engine/Texture.h"


class Switch : public PM::GameObject
{
public:
	Switch(std::string name, glm::vec2 pos, glm::vec2 size = { 80,80 }, std::string filepath = "assets/GameObject/Switch_On.png");

	EntityType GetEntityType() const override { return EntityType::Switch; }
	std::string GetEntityTypeName() const override {return "Switch";}

	virtual void On();
	virtual void Off();
	bool GetIsOn() { return IsOn; }
	void CollisionEnter(Entity*) override;
	bool CanCollideWith(EntityType objectBType) override;
private:
	bool IsOn{true};
};

class LaserSwitch : public Switch
{
public:
	LaserSwitch(std::string name, glm::vec2 pos);

	void On() override;
	void Off() override;
private:
};