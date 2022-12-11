/*--------------------------------------------------------------
File Name: Enemy_soldier_AI.cpp
Project: Spy The Man
Author: Seongwon Jang
Creation date: 9/24/2021
-----------------------------------------------------------------*/
#pragma once
#include "../../Engine/GameObject.h"
#include<glm/glm.hpp>

#include "../../Engine/Texture.h"


class Clothes : public PM::GameObject
{
public:
	Clothes(std::string name, glm::vec2 pos, DisguiseList d, glm::vec2 size = CLOTHESSIZE, std::string filepath =CLOTHESPATH);
	~Clothes();

	void Update(double dt) override;

	EntityType GetEntityType() const override {return EntityType::Clothes;}
	std::string GetEntityTypeName() const override {return "Clothes";}

};
