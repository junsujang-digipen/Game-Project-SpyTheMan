/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Find.cpp
Project: Spy the Man
Author: Sangbeom Kim
Creation date: 4/25/2022
-----------------------------------------------------------------*/
#include "Find.h"
#include "../../Engine/Engine.h"
#include "../../Engine/Poodle.h"
#include "../Engine/World.h"

Find::Find(glm::vec2 pos) : GameObject("Find", pos)
{}

void Find::Update(double dt) {
	GameObject::Update(dt);
	if (size >= 10000 && sizeadd > 0) {
		sizeadd -= 0.5;
	}
	if (sizeadd <= 0) {
		sizeadd = 0;
	}
	if (sizeadd <= 0 && dead == false) {
		dead = true;
	}
	if (dead == true) {
		alpha -= 10;
	}
	if (alpha <= 0) {
		this->Destroy();
	}
	size += sizeadd;
}

void Find::Draw(math::TransformMatrix& displayMatrix) const {
	GameObject::Draw(displayMatrix);
	PM::push_settings();
	PM::apply_matrix(displayMatrix.matrix);
	PM::set_fill_color(255, 255, 255, alpha);
	PM::draw_ellipse(GetPosition().x, GetPosition().y, size, size);

	PM::set_fill_color(255, 0, 0);
	PM::no_outline();
	for (auto const& i : Engine::GetGSComponent<PM::World>()->GetAllNPC()) {
		if (glm::length(GetPosition() - i->GetCenter()) <= size / 2 + 100 && glm::length(GetPosition() - i->GetCenter()) >= size / 2 - 500) {
			PM::draw_ellipse(i->GetCenter().x, i->GetCenter().y, 100, 100);
		}
	}

	PM::pop_settings();
}

EntityType Find::GetEntityType() const
{
	return EntityType::Find;
}

std::string Find::GetEntityTypeName() const
{
	return "Find";
}

