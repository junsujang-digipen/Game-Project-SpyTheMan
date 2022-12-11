/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Find.h
Project: Spy the Man
Author: Sangbeom Kim
Creation date: 4/29/2022
-----------------------------------------------------------------*/
#pragma once
#include "../../Engine/GameObject.h"
#include "..\Engine\Rect.h"

namespace PM
{
	class Texture;
}

class Find : public PM::GameObject
{
public:
	Find(glm::vec2 pos);
	void Update(double dt) override;
	void Draw(math::TransformMatrix& displayMatrix) const override;

	EntityType GetEntityType() const override;
	std::string GetEntityTypeName() const override;

private:
	math::irect2 collisionSize{ {-1500, 0}, {1500, 100} };
	double size = 10;
	double sizeadd = 30;
	double alpha = 150;
	bool dead = false;
};
