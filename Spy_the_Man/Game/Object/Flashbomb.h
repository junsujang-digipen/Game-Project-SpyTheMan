/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Flashbomb.h
Project: Spy the Man
Author: Sangbeom Kim
Creation date: 4/25/2022
-----------------------------------------------------------------*/
#pragma once
#include "../../Engine/GameObject.h"
#include "..\Engine\Rect.h"

namespace PM
{
	class Texture;
}

class Flashbomb : public PM::GameObject
{
public:
	Flashbomb(glm::vec2 pos);
	void Update(double dt) override;
	void Draw(math::TransformMatrix& displayMatrix) const override;

	EntityType GetEntityType() const override;
	std::string GetEntityTypeName() const override;

private:
	math::irect2 collisionSize{ {-1000, 0}, {1000, 100} };
	double timer = 3;
	double size = 10;
	double sizeadd = 50;
	double alpha = 150;
	bool dead = false;
};
