/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Trap.cpp
Project: Spy the Man
Author: Sangbeom Kim
Creation date: 4/24/2022
-----------------------------------------------------------------*/
#include "Trap.h"

#include "../../Engine/Engine.h"
#include "../../Engine/Poodle.h"

Trap::Trap(glm::vec2 pos) : GameObject("Trap", pos)
{
	this->AddGOComponent(new PM::RectCollision(collisionSize, this));
}

void Trap::Update(double dt) {
	GameObject::Update(dt);
}

void Trap::Draw(math::TransformMatrix& displayMatrix) const {
	GameObject::Draw(displayMatrix);
	PM::push_settings();
	PM::apply_matrix(displayMatrix.matrix);
	PM::no_outline();
	PM::set_fill_color(128, 0, 128, 200);
	PM::draw_rect(GetPosition().x, GetPosition().y, 500, 100);
	PM::pop_settings();
}

bool Trap::CanCollideWith(EntityType objectBType) {
	switch (objectBType) {
	case EntityType::Basic_NPC: [[fallthrough]];
	case EntityType::Enemy_NPC: [[fallthrough]];
	case EntityType::Target_NPC: [[fallthrough]];
	case EntityType::Worker_NPC:
		return true;
		break;
	}
	return false;
}

void Trap::CollisionStay(PM::Entity* obj) {
	math::rect2 collideRect = obj->GetGOComponent<PM::RectCollision>()->GetWorldCoorRect();
	math::rect2 playerRect = GetGOComponent<PM::RectCollision>()->GetWorldCoorRect();
}

EntityType Trap::GetEntityType() const
{
	return EntityType::Trap;
}

std::string Trap::GetEntityTypeName() const
{
	return "Trap";
}

