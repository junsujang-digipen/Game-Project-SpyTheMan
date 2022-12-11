/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Project: Spy the Man
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include "Goal.h"
#include "../Engine/Collision.h"
#include "../Engine/Engine.h"
#include "../Engine/World.h"
Goal::Goal(glm::vec2 pos) : GameObject("Goal",pos, { 270,270 },"assets/GameObject/goal.png")
{
	AddGOComponent(new PM::RectCollision({{0,0},{200,200}}, this));
}

void Goal::CollisionEnter(Entity*)
{
	Engine::GetGSComponent<PM::World>()->SetIsClear(true);
}


bool Goal::CanCollideWith(EntityType objectBType)
{
	return objectBType == EntityType::Player;
}


