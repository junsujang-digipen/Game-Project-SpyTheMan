/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Project: Spy the Man
Author: Seongwon Jang
-----------------------------------------------------------------*/
#pragma once
#include <list>


#include "../../Engine/GameObject.h"


class Goal : public PM::GameObject
{
public:
	Goal(glm::vec2 pos);
	void CollisionEnter(Entity*) override;
	bool CanCollideWith(EntityType objectBType) override;
	EntityType GetEntityType() const override { return EntityType::Goal; }
	std::string GetEntityTypeName() const override { return "Goal"; }

};



