/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Push.cpp
Project: Spy the Man
Author: Sangbeom Kim
Creation date: 4/26/2022
-----------------------------------------------------------------*/
#pragma once
#include "../../Engine/GameObject.h"
#include "..\Engine\Rect.h"

namespace PM
{
	class Texture;
}

class Push : public PM::GameObject
{
public:
	Push(glm::vec2 pos, float length, bool left);
	void Update(double dt) override;
	void Draw(math::TransformMatrix& displayMatrix) const override;
	bool IsLeft() { return left; }

	EntityType GetEntityType() const override;
	std::string GetEntityTypeName() const override;

private:
	float length;
	double timer = 5;
	bool left;
};
