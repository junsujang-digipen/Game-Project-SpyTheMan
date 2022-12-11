/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Building.cpp
Project: CS200
Author: Dong-A Choi
Creation date: 12/10/2021
-----------------------------------------------------------------*/
#include "Building.h"

#include "Player.h"
#include "../Engine/Poodle.h"
#include "../Engine/GameObject.h"

Building::Building(glm::dvec2 pos, int level) : position(pos), levels(level) {}

void Building::Draw(math::TransformMatrix cameraMatrix)
{
	PM::push_settings();
	PM::apply_matrix(cameraMatrix.matrix);

	//we will change this part to Image.
	//For now use draw_rect. So I draw only one window and not look pretty. It will be changed.
	switch (static_cast<Levels>(levels))
	{
	case Levels::Level0:
		PM::set_fill_color(180, 180, 170);
		PM::draw_rect(position.x, position.y, size.x, size.y);
		windowDraw({ position.x, 280 }, windowSize);
		windowDraw({ position.x + 160, 280 }, windowSize);
		break;
	case Levels::Level1:
		//building
		PM::set_fill_color(236, 230, 204);
		PM::draw_rect(position.x, position.y, size.x, size.y);
		windowDraw({ position.x, 280 }, windowSize);
		windowDraw({ position.x + 160, 280 }, windowSize);
		break;
	case Levels::Level2:
		PM::set_fill_color(236, 230, 204);
		PM::draw_rect(position.x, position.y, size.x, size.y);
		windowDraw({ position.x, 280 }, windowSize);
		windowDraw({ position.x + 160, 280 }, windowSize);
		break;
	case Levels::Level3:
		PM::set_fill_color(236, 230, 204);
		PM::draw_rect(position.x, position.y, size.x, size.y);
		windowDraw({ position.x, 280 }, windowSize);
		windowDraw({ position.x + 160, 280 }, windowSize);
		break;
	case Levels::Level4:
		PM::set_fill_color(236, 230, 204);
		PM::draw_rect(position.x, position.y, size.x, size.y);
		windowDraw({ position.x, 280 }, windowSize);
		windowDraw({ position.x + 160, 280 }, windowSize);
		break;
	case Levels::Level5:
		PM::set_fill_color(236, 230, 204);
		PM::draw_rect(position.x, position.y, size.x, size.y);
		windowDraw({ position.x, 280 }, windowSize);
		windowDraw({ position.x + 160, 280 }, windowSize);
		break;
	case Levels::Quit:
		PM::set_fill_color(150, 150, 160);
		PM::draw_rect(position.x, position.y, size.x - 100, size.y - 200);
		break;
	case Levels::Skill:
		PM::set_fill_color(170, 170, 180);
		PM::draw_rect(position.x + 10, position.y, size.x / 2 + 10, 500);
		windowDraw({ position.x + 20, 250 }, windowSize);
	}
	PM::pop_settings();
}

void Building::windowDraw(glm::dvec2 pos, glm::dvec2 win_size)
{
	PM::set_fill_color(168, 204, 215);
	PM::set_line_width(3);
	PM::draw_rect(pos.x + 10, pos.y + 20, win_size.x, win_size.y);
}
