/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Project: Spy the Man
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include "Wall.h"
#include  "../../Engine/Collision.h"
#include  "../../Engine/Engine.h"
Wall::Wall(std::string name, glm::vec2 pos) : GameObject(name,pos)
{
	currState = &state_nothing;
	this->SetSize( glm::vec2{ 50,475 });
	AddGOComponent(new PM::RectCollision(math::irect2{ {0,0},GetSize() },this));
}

Wall::Wall(std::string name, glm::vec2 pos,glm::vec2 size) : GameObject(name, pos)
{
	currState = &state_nothing;
	this->SetSize(size);
	AddGOComponent(new PM::RectCollision(math::irect2{ {0,0},GetSize() }, this));
}

Wall::~Wall(){}

void Wall::Update(double dt/*dt*/)
{
	GameObject::Update(dt);
}

void Wall::Draw(math::TransformMatrix& displayMatrix) const
{
	glm::vec2 Wposition = GetPosition();
	glm::vec2 Wsize = GetSize();
	PM::push_settings();
	PM::no_outline();
	PM::set_fill_color(100, 100, 100, 255);
	PM::apply_matrix(displayMatrix.matrix);
	PM::draw_rect(Wposition.x, Wposition.y, Wsize.x, Wsize.y);
	PM::pop_settings();
	GameObject::Draw(displayMatrix);
}


EntityType Wall::GetEntityType() const
{
	return EntityType::Wall;
}

std::string Wall::GetEntityTypeName() const
{
	return "Wall";
}
