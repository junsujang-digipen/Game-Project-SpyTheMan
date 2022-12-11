/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Drone.cpp
Project: Spy the Man
Author: Sangbeom Kim
Creation date: 3/5/2021
-----------------------------------------------------------------*/
#include "../Engine/Engine.h"
#include "../Engine/Poodle.h"
#include "../Engine/Collision.h"
#include "../Engine/Sprite.h"
#include "../Engine/Rect.h"
#include "../Engine/IOManager/ObjectData.h"
#include "../Engine/SoundManager.h"
#include "Drone.h"


#include "Object/ActivateVision.h"
#include "Sequence/DroneSequence.h"


Drone::Drone(glm::vec2 position) : GameObject("Drone", position),
                                     moveLeftKey(PM::InputKey::Keyboard::A), 
                                     moveRightKey(PM::InputKey::Keyboard::D),
									 finishKey(PM::InputKey::Keyboard::X),
                                     isFinish(false) {
	this->AddGOComponent(new PM::RectCollision(collisionSize, this));
	AddGOComponent(new PM::Sprite("assets/drone.json", this->GetEntityType()));
	SetSize(droneSize);
	currState = &stateIdle;
	currState->Enter(this);
	Engine::GetSoundManager().PlaySound("droneStart", position);
	AddGOComponent(new ActivateVision(this));
}

void Drone::Update(double dt) {
	GameObject::Update(dt);
	if (isFinish == false) {
		Engine::GetSoundManager().setSoundListener(GetCenter());
	}
	if (finishKey.IsKeyDown() == true) {
		isFinish = true;
	}
	if (IsFaceLeft() == true) {
		camPos = { GetCenter().x - 700, GetCenter().y + 100 };
		idlePos = { GetCenter().x, GetCenter().y + 100 };
	}
	else {
		camPos = { GetCenter().x + 700, GetCenter().y + 100 };
		idlePos = { GetCenter().x, GetCenter().y + 100 };
	}
}

void Drone::Draw(math::TransformMatrix& displayMatrix) const {	
	GameObject::Draw(displayMatrix);
}

void Drone::CollisionEnter(Entity* obj) {
	switch (obj->GetEntityType()) {
	case EntityType::Lazer: 
		SetFinish(true); 
		break;
	default: 
		break;
	}
}

bool Drone::CanCollideWith(EntityType objectBType) {
	if (IsFinish() == false) {
		switch (objectBType) {
		case EntityType::Wall: [[fallthrough]];
		case EntityType::Lazer:
			return true;
			break;
		}
	}
	return false;
}

void Drone::CollisionStay(PM::Entity* obj) {
	math::rect2 collideRect = obj->GetGOComponent<PM::RectCollision>()->GetWorldCoorRect();
	math::rect2 playerRect = GetGOComponent<PM::RectCollision>()->GetWorldCoorRect();
	if (obj->GetEntityType() == EntityType::SideDoor || (obj->GetEntityType() == EntityType::Wall)) {
		if (playerRect.Right() > collideRect.Left() && playerRect.Left() < collideRect.Left()) {
			SetPosition({ collideRect.Left() - playerRect.Size().x, GetPosition().y });
			SetVelocity( {0, GetVelocity().y} );
		}
		else if (playerRect.Left() < collideRect.Right() && playerRect.Right() > collideRect.Right()) {
			SetPosition({ collideRect.Right(), GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
		}
	}
}

void Drone::State_Idle::Enter(GameObject* object) {
	Drone* drone = static_cast<Drone*>(object);
	drone->GetGOComponent<PM::Sprite>()->PlayAnimation(1);
}

void Drone::State_Idle::Update(GameObject* object, double) {
	Drone* drone = static_cast<Drone*>(object);
	Engine::GetCamera().bindTargetPos(drone->idlePos);
}

void Drone::State_Idle::TestForExit(GameObject* object) {
	Drone* drone = static_cast<Drone*>(object);
	if (drone->moveLeftKey.IsKeyDown() == true || drone->moveRightKey.IsKeyDown() == true) {
		drone->ChangeState(&drone->stateMoving);
	}
	if (drone->isFinish == true) {
		drone->ChangeState(&drone->stateFinish);
		drone->SetVelocity({ 0, 0 });
	}
}

void Drone::State_Moving::Enter(GameObject* object) {
	Drone* drone = static_cast<Drone*>(object);
	drone->GetGOComponent<PM::Sprite>()->PlayAnimation(2);
}

void Drone::State_Moving::Update(GameObject* object, double) {
	Drone* drone = static_cast<Drone*>(object);
	if (drone->GetVelocity() == glm::vec2{ 0, 0 }) {
		Engine::GetCamera().bindTargetPos(drone->idlePos);
	}
	else {
		Engine::GetCamera().bindTargetPos(drone->camPos);
	}

	if (drone->moveLeftKey.IsKeyDown() == true) {
		drone->SetVelocity({ -Drone::xVel, 0 });
		drone->FaceLeft(true);
	}
	if (drone->moveRightKey.IsKeyDown() == true) {
		drone->SetVelocity({ Drone::xVel, 0 });
		drone->FaceLeft(false);
	}
}

void Drone::State_Moving::TestForExit(GameObject* object) {
	Drone* drone = static_cast<Drone*>(object);
	if (drone->moveLeftKey.IsKeyDown() == false && drone->moveRightKey.IsKeyDown() == false) {
		drone->ChangeState(&drone->stateIdle);
		drone->SetVelocity({ 0, 0 });
	}
	if (drone->isFinish == true) {
		drone->ChangeState(&drone->stateFinish);
		drone->SetVelocity({ 0, 0 });
	}
}

void Drone::State_Finish::Enter(GameObject* object) {
	Drone* drone = static_cast<Drone*>(object);
	drone->GetGOComponent<PM::Sprite>()->PlayAnimation(3);
}

void Drone::State_Finish::Update(GameObject*, double) {}

void Drone::State_Finish::TestForExit(GameObject* object) {
	Drone* drone = static_cast<Drone*>(object);
	if (drone->GetGOComponent<PM::Sprite>()->IsAnimationDone() == true) {
		drone->ChangeState(&drone->stateDead);
	}
}

void Drone::State_Dead::Enter(GameObject* object) {
	Drone* drone = static_cast<Drone*>(object);
	drone->GetGOComponent<PM::Sprite>()->PlayAnimation(4);
}

void Drone::State_Dead::Update(GameObject*, double) {}

void Drone::State_Dead::TestForExit(GameObject* object) {
	Drone* drone = static_cast<Drone*>(object);
	if (drone->isFinish == false) {
		drone->ChangeState(&drone->stateIdle);
	}
}