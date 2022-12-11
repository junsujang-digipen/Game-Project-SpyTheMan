/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Project: Spy the Man
Author: Seongwon Jang
-----------------------------------------------------------------*/
#pragma once
#include "../../Engine/GameObject.h"
#include<glm/glm.hpp>

#include "../../Engine/Texture.h"


class Stair : public PM::GameObject
	{
	public:
		Stair(std::string name, glm::vec2 pos, std::string targetName,bool isDown = false, glm::vec2 size = {140, 280}, std::string filepath = "assets/GameObject/Stair/upStair.png");
		GameObject* GetTarget() const;
		
		void Update(double dt) override;

		EntityType GetEntityType() const override { return EntityType::Stair; }
		std::string GetEntityTypeName() const override { return std::string("Stair"); }

		void CollisionStay(Entity*) override;
		bool CanCollideWith(EntityType objectBType) override { return objectBType == EntityType::Player; }
	private:
		std::string TargetName;
		mutable GameObject* TargetStair{nullptr};
	};
