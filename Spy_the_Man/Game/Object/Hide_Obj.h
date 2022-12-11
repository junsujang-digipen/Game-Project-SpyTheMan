/*--------------------------------------------------------------
File Name: Enemy_soldier_AI.cpp
Project: Spy The Man
Author: Seongwon Jang
Creation date: 9/24/2021
-----------------------------------------------------------------*/
#pragma once
#include <list>
#include "../../Engine/GameObject.h"
#include<glm/glm.hpp>

namespace PM
{
	class Texture;
}

class Hide_Obj : public PM::GameObject
{
public:

	Hide_Obj(std::string name, glm::vec2 pos, glm::vec2 size = HIDEOBJSIZE, std::string filepath = HIDEOBJPATH);

	void Update(double dt) override;

	bool CanCollideWith(EntityType objectBType) override;
	void CollisionStay(Entity*) override;

	EntityType GetEntityType() const override {return EntityType::Hide_Obj;}
	std::string GetEntityTypeName() const override {return std::string("Hide_Obj");}
};