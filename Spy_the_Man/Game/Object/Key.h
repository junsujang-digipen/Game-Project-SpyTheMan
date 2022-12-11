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


class Key : public PM::GameObject
{
public:
	Key(std::string name, glm::vec2 pos, std::string targetName, glm::vec2 size = KEYSIZE, std::string filepath = KEYPATH);

	void Update(double dt) override;

	EntityType GetEntityType() const override { return EntityType::Key; }
	std::string GetEntityTypeName() const override {return "Key";}

};
