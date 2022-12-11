/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Flashbomb.cpp
Project: Spy the Man
Author: Sangbeom Kim
Creation date: 4/25/2022
-----------------------------------------------------------------*/
#include "Flashbomb.h"

#include "../../Engine/Engine.h"
#include "../../Engine/Poodle.h"

Flashbomb::Flashbomb(glm::vec2 pos) : GameObject("Flashbomb", pos)
{
	this->AddGOComponent(new PM::RectCollision(collisionSize, this));
}

void Flashbomb::Update(double dt) {
	GameObject::Update(dt);
	timer -= dt;
	if (size >= 950 && sizeadd > 0) {
		sizeadd -= 5;
	}
	if (sizeadd <= 0) {
		sizeadd = 0;
	}
	if (timer <= 0 && dead == false) {
		dead = true;
		this->GetGOComponent<PM::RectCollision>()->SetCollision({ {0, 0}, {0, 0} });
	}
	if (dead == true) {
		alpha -= 10;
	}
	if (alpha <= 0) {
		this->Destroy();
	}
	size += sizeadd;
}

void Flashbomb::Draw(math::TransformMatrix& displayMatrix) const {
	GameObject::Draw(displayMatrix);
	PM::push_settings();
	PM::apply_matrix(displayMatrix.matrix);
	PM::set_fill_color(255, 255, 255, alpha);
	PM::draw_ellipse(GetPosition().x, GetPosition().y, size, size);
	PM::pop_settings();
}

EntityType Flashbomb::GetEntityType() const
{
	return EntityType::Flashbomb;
}

std::string Flashbomb::GetEntityTypeName() const
{
	return "Flashbomb";
}

