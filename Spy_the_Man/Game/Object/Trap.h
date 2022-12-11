/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Trap.h
Project: Spy the Man
Author: Sangbeom Kim
Creation date: 4/24/2022
-----------------------------------------------------------------*/
#pragma once
#include "../../Engine/GameObject.h"
#include "..\Engine\Rect.h"

namespace PM
{
	class Texture;
}

class Trap : public PM::GameObject
{
public:
	Trap(glm::vec2 pos);
	void Update(double dt) override;
	void Draw(math::TransformMatrix& displayMatrix) const override;
	void CollisionStay(PM::Entity* obj) override;
	bool CanCollideWith(EntityType objectBType) override;

	EntityType GetEntityType() const override;
	std::string GetEntityTypeName() const override;

private:
	math::irect2 collisionSize{ {0, 0}, {500, 100} };
	double timer = 3;
};
