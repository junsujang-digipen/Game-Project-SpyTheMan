/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Push.cpp
Project: Spy the Man
Author: Sangbeom Kim
Creation date: 4/24/2022
-----------------------------------------------------------------*/
#include "Push.h"

#include "../../Engine/Engine.h"
#include "../../Engine/Poodle.h"

Push::Push(glm::vec2 pos, float length, bool left) : left(left), length(length), GameObject("Push", pos)
{
	this->AddGOComponent(new PM::RectCollision(math::irect2{ {0, 0}, {length, 400} }, this));
}

void Push::Update(double dt) {
	GameObject::Update(dt);
	timer -= dt;
	if (timer <= 0) {
		this->GetGOComponent<PM::RectCollision>()->SetCollision({ {0, 0}, {0, 0} });
	}
	if (timer <= -0.1) {
		this->Destroy();
	}
}

void Push::Draw(math::TransformMatrix& displayMatrix) const {
	GameObject::Draw(displayMatrix);
	PM::push_settings();
	PM::apply_matrix(displayMatrix.matrix);
	PM::no_outline();
	PM::set_fill_color(0, 242, 222, 100);
	PM::draw_rect(GetPosition().x, GetPosition().y, length, 250);
	PM::pop_settings();
}

EntityType Push::GetEntityType() const
{
	return EntityType::Push;
}

std::string Push::GetEntityTypeName() const
{
	return "Push";
}

