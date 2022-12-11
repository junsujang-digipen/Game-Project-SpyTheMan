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


class Wall : public PM::GameObject
{
public:
	Wall(std::string name, glm::vec2 pos);
	Wall(std::string name, glm::vec2 pos,glm::vec2 size);
	~Wall();
	void Update(double dt) override;
	void Draw(math::TransformMatrix& displayMatrix) const override;

	EntityType GetEntityType() const override;
	std::string GetEntityTypeName() const override;
	
};
