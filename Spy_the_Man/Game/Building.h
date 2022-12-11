/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Building.h
Project: CS200
Author: Dong-A Choi
Creation date: 12/10/2021
-----------------------------------------------------------------*/
#pragma once
#include <glm/vec2.hpp>
#include "../Engine/TransformMatrix.h"

enum class Levels
{
	Level0,
	Level1,
	Level2,
	Level3,
	Level4,
	Level5,
	Quit,
	Skill
};

class Building
{
public:
	Building(glm::dvec2 pos, int level);
	void Draw(math::TransformMatrix cameraMatrix);
	void windowDraw(glm::dvec2 position, glm::dvec2 windowSize);
	void CollisionCheck();

	glm::dvec2 getPosition() {return position;}
	glm::dvec2 getSize() { return size; }

private:
	glm::dvec2 position;
	glm::dvec2 size = {300,550};
	glm::dvec2 windowSize = { 120,120 };
	int levels;
};
