/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObject.cpp
Project: CS230
Author: Kevin Wright
Creation date: 2/14/2021
-----------------------------------------------------------------*/
#include "Engine.h"
#include "GameObject.h"
#include "Sprite.h"
#include "ShowCollision.h"
#include "../Game/Object/ButtonManager.h"


PM::GameObject::GameObject(std::string name, glm::vec2 position, glm::vec2 size, std::string filepath) :
	Entity(name,position, size),
	currState(&state_nothing),
	destroyObject(false)
{
	if (filepath != "nothing") { imagePtr = Engine::GetTextureManager().Load(filepath); }
	AddGOComponent(new ButtonManager(this));
}


PM::GameObject::~GameObject() {

}

void PM::GameObject::Update(double dt) {
	currState->Update(this, dt);
	glm::vec2 Vel = GetVelocity();
	if (Vel.x != 0 || Vel.y != 0) {
		UpdatePosition(Vel * glm::vec2{dt,dt});
	}
	UpdateGOComponents(dt);

	currState->TestForExit(this);
}

void PM::GameObject::Draw(math::TransformMatrix& displayMatrix) const
{
	if(imagePtr != nullptr)
	{
		imagePtr->Draw(displayMatrix, GetPosition(), GetSize());
	}
	Entity::Draw(displayMatrix);
}


void PM::GameObject::ChangeState(State* newState) {

	currState = newState;
	currState->Enter(this);
}


void PM::GameObject::SetLayer(int l)
{
	layer = l;
	updateChunk = true;
}

int PM::GameObject::GetLayer() const
{
	return layer;
}

void PM::GameObject::ChangeImage(std::string filepath)
{
	imagePtr = Engine::GetTextureManager().Load(filepath);
}